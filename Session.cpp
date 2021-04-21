/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 23:21:37 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/04/21 20:40:31 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Session.hpp"

#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

#include "webserv_settings.hpp"
#include "webserv_utils.hpp"

std::map<std::string, std::string> Session::map_mime_ext_;
std::map<std::string, std::string> Session::map_ext_mime_;

/*
** constructor
**
** initialize fd and status
**    - status is initialized SESSION_FOR_CLIENT_RECV first
*/

Session::Session(int sock_fd, const MainConfig& main_config)
    : sock_fd_(sock_fd),
      retry_count_(0),
      status_(SESSION_FOR_CLIENT_RECV),
      main_config_(main_config),
      server_config_(NULL),
      location_config_(NULL) {
  initMapMineExt();
  updateConnectionTime();
}

Session::~Session() { close(sock_fd_); };

/*
** assignation operator overload
**
** will be used only in list<Session>
*/

Session& Session::operator=(const Session& rhs) {
  if (this == &rhs) {
    return *this;
  }
  sock_fd_ = rhs.sock_fd_;
  status_ = rhs.status_;
  // request_ = rhs.request_;
  // response_ = rhs.response_;
  // cgi_handler_ = rhs.cgi_handler_;
  cgi_pid_ = rhs.cgi_pid_;
  return *this;
}

/*
** getters
*/

int Session::getSockFd() const { return sock_fd_; }
int Session::getFileFd() const { return file_fd_; }
const SessionStatus& Session::getStatus() const { return status_; }
in_addr_t Session::getIp() const { return ip_; };
uint16_t Session::getPort() const { return port_; };

/*
** setFdToSelect
** set sessions fd for select
*/

int Session::setFdToSelect(fd_set* rfds, fd_set* wfds) {
  switch (status_) {
    case SESSION_FOR_CLIENT_RECV:
      FD_SET(sock_fd_, rfds);
      return sock_fd_;
    case SESSION_FOR_FILE_READ:
      FD_SET(file_fd_, rfds);
      return file_fd_;
    case SESSION_FOR_FILE_WRITE:
      FD_SET(file_fd_, wfds);
      return file_fd_;
    case SESSION_FOR_CGI_WRITE:
      FD_SET(cgi_handler_.getInputFd(), wfds);
      return cgi_handler_.getInputFd();
    case SESSION_FOR_CGI_READ:
      FD_SET(cgi_handler_.getOutputFd(), rfds);
      return cgi_handler_.getOutputFd();
    case SESSION_FOR_CLIENT_SEND:
      FD_SET(sock_fd_, wfds);
      return sock_fd_;
    default:
      return 0; /* return 0 means that max_fd always becomes initial value*/
  }
}

/*
** checkSelectedAndExecute
** check the session is selected by select syscall using FD_ISSET.
**
** Return val
**  1: session was selected and executed correctly
**  0: session was not selected
**  -1: session was selected but to close
**  -2: session was not selected and to close
*/

int Session::checkSelectedAndExecute(fd_set* rfds, fd_set* wfds) {
  if (status_ == SESSION_FOR_CLIENT_RECV && FD_ISSET(sock_fd_, rfds)) {
    if (receiveRequest() == -1) {
      return (-1);
    } else {
      std::cout << "[webserv] received request data" << std::endl;
      updateConnectionTime();
      return (1);
    }
  } else if (status_ == SESSION_FOR_FILE_READ && FD_ISSET(file_fd_, rfds)) {
    if (readFromFile() == -1) {
      return (-1);
    } else {
      std::cout << "[webserv] read data from file" << std::endl;
      return (1);
    }
  } else if (status_ == SESSION_FOR_FILE_WRITE && FD_ISSET(file_fd_, wfds)) {
    if (writeToFile() == -1) {
      return (-1);
    } else {
      std::cout << "[webserv] write data to file" << std::endl;
      return (1);
    }
  } else if (status_ == SESSION_FOR_CGI_WRITE &&
             FD_ISSET(cgi_handler_.getInputFd(), wfds)) {
    if (writeToCgi() == -1) {
      return (-1);
    } else {
      std::cout << "[webserv] wrote data to cgi" << std::endl;
      return (1);
    }
  } else if (status_ == SESSION_FOR_CGI_READ &&
             FD_ISSET(cgi_handler_.getOutputFd(), rfds)) {
    if (readFromCgi() == -1) {
      return (-1);
    } else {
      std::cout << "[webserv] read data from cgi" << std::endl;
      return (1);
    }
  } else if (status_ == SESSION_FOR_CLIENT_SEND && FD_ISSET(sock_fd_, wfds)) {
    if (sendResponse() != 0) {
      return (-1);
    } else {
      updateConnectionTime();
      return (1);
    }
  }

  // check connection time out
  if ((status_ == SESSION_FOR_CLIENT_RECV ||
       status_ == SESSION_FOR_CLIENT_SEND) &&
      checkConnectionTimeOut()) {
    return (-2);
  }
  return (0);
}

/*
** PRIVATE FUNCTIONS
*/

/*
** connection manager
*/

void Session::updateConnectionTime() {
  struct timeval tv;
  if (gettimeofday(&tv, NULL) == -1) {
    time_last_connect_ = 0;
  } else {
    time_last_connect_ = tv.tv_sec;
  }
}

bool Session::checkConnectionTimeOut() const {
  struct timeval tv;
  if (gettimeofday(&tv, NULL) == -1) {
    return true;
  }

  if (time_last_connect_ + KEEP_ALIVE_SEC < tv.tv_sec) {
    return true;
  }
  return false;
}

// connect to list
void Session::resetAll() {
  retry_count_ = 0;
  request_.resetAll();
  response_.resetAll();
}

/*
** request handler
*/

// call receive and parse request and client
int Session::receiveRequest() {
  int ret;
  ret = request_.receive(sock_fd_);
  if (ret == REQ_ERR_RECV) {
    if (retry_count_ == RETRY_TIME_MAX) {
      // then try to send return internal server error
      createErrorResponse(HTTP_500);
    } else {
      retry_count_++;
    }
    return 0;
  } else if (ret == REQ_CLOSE_CON) {
    return -1;
  }
  retry_count_ = 0;
  return checkReceiveReturn(ret);
}

// check return value of request_.receive(sock_fd_);
int Session::checkReceiveReturn(int ret) {
  /* firstly check the return value is ERROR or NOT*/
  if (ret == REQ_ERR_BAD_REQUEST) {
    createErrorResponse(HTTP_400);
  } else if (ret == REQ_ERR_HTTP_VERSION) {
    createErrorResponse(HTTP_505);
  } else if (ret == REQ_ERR_LEN_REQUIRED) {
    createErrorResponse(HTTP_411);
  }
  /*
  ** Then check the content-length,
  ** if it's 0 (no body), return 0
  ** else if it'is larger than client max body size return HTTP413(Payload Too
  *Large)
  */
  else if (ret == REQ_FIN_PARSE_HEADER) {
#ifndef UNIT_TEST
    setupServerAndLocationConfig();  // To get server and location config
#endif
    if (!request_.getFlgChunked() && (request_.getContentLength() == 0)) {
      startCreateResponse();
    } else if (location_config_ &&
               request_.getContentLength() >
                   location_config_->getClientMaxBodySize()) {
      createErrorResponse(HTTP_413);
#ifdef UNIT_TEST
      return 4131;  // just for unit test
#endif
    } else if (server_config_ && request_.getContentLength() >
                                     server_config_->getClientMaxBodySize()) {
      createErrorResponse(HTTP_413);
#ifdef UNIT_TEST
      return 4132;  // just for unit test
#endif
    } else if (request_.getContentLength() >
               main_config_.getClientMaxBodySize()) {
      createErrorResponse(HTTP_413);
#ifdef UNIT_TEST
      return 4133;  // just for unit test
#endif
      /* Finished receiving then start create response*/
    }
  } else if (ret == REQ_FIN_RECV) {
    startCreateResponse();
  }
  return 0;
}

/*
** response creation manager
*/

// check request, load proper config and start creating response
void Session::startCreateResponse() {
  // check connection header
  std::map<std::string, std::string>::const_iterator itr;
  itr = request_.getHeaders().find("connection");
  if (itr != request_.getHeaders().end() && itr->second == "close") {
    response_.setConnectionToClose();
  }

  // case GET
  if ((request_.getMethod() == "GET" && isMethodAllowed(HTTP_GET)) ||
      (request_.getMethod() == "HEAD" && isMethodAllowed(HTTP_HEAD))) {
    startCreateResponseToGet();
    return;
  }

  // case POST
  if (request_.getMethod() == "POST" && isMethodAllowed(HTTP_POST)) {
    startCreateResponseToPost();  // write to file or cgi
    return;
  }

  // case others (PUT, DELETE and TRACE)
  // not inpl them for now
  createErrorResponse(HTTP_405);
}

// create Response to GET
void Session::startCreateResponseToGet() {
  std::string filepath;
  struct stat pathstat;

  // check path includes cgi extension
  const std::string cgiuri = findCgiPathFromUri();
  if (!cgiuri.empty()) {
    filepath = findRoot() + cgiuri;
    if (filepath.empty()) {
      createErrorResponse(HTTP_404);
    } else {
      createCgiProcess(filepath, cgiuri);
    }
    return;
  }

  // find file
  filepath = findRoot() + request_.getUri();
  if (stat(filepath.c_str(), &pathstat) == -1) {
    createErrorResponse(HTTP_404);
    return;
  }

  // check file type
  if (S_ISREG(pathstat.st_mode)) {
    if (isCgiFile(filepath)) {
      createCgiProcess(filepath, "");
    } else {
      startReadingFromFile(filepath);
    }
    return;
  } else if (S_ISDIR(pathstat.st_mode)) {
    std::string res = findFileFromDir(filepath);
    if (res.empty()) {
      startDirectoryListing(filepath);
    } else {
      startReadingFromFile(filepath + res);
    }
    return;
  }
  createErrorResponse(HTTP_404);
}

void Session::startCreateResponseToPost() {
  // check path includes cgi extension
  const std::string cgiuri = findCgiPathFromUri();
  if (!cgiuri.empty()) {
    std::string filepath = findRoot() + cgiuri;
    if (filepath.empty()) {
      createErrorResponse(HTTP_404);
    } else {
      createCgiProcess(filepath, cgiuri);
    }
    return;
  }

  // try to write to file if not cgi
  startWritingToFile();
}

// check HTTP Request method are avairable
bool Session::isMethodAllowed(HTTPMethodFlag method) const {
  if (location_config_) {
    return (location_config_->getLimitExcept() & method);
  }
  if (server_config_) {
    return (server_config_->getLimitExcept() & method);
  }
  return (main_config_.getLimitExcept() & method);
}

// create error response message
// yet not enough
void Session::createErrorResponse(HTTPStatusCode http_status) {
  // find file
  // todo
  // response_.createStatusLine(http_status);
  // startReadingFromFile

  // createDefaultErrorResponse
  response_.createDefaultErrorResponse(http_status);
  status_ = SESSION_FOR_CLIENT_SEND;
}

// send response
int Session::sendResponse() {
  ssize_t n;

  n = response_.sendData(sock_fd_);
  if (n == -1) {
    std::cout << "[error] failed to send response" << std::endl;
    if (retry_count_ == RETRY_TIME_MAX) {
      std::cout << "[error] close connection" << std::endl;
      close(sock_fd_);
      return -1;  // return -1 if error (this session will be closed)
    }
    retry_count_++;
    return 0;
  }
  if (n == 0) {
    if (response_.isConnectionToClose()) {
      return 1;  // return 1 if all data sent and is not keep alive (this
                 // session will be closed)
    }
    resetAll();                         // reset session
    status_ = SESSION_FOR_CLIENT_RECV;  // wait for next request
  }
  retry_count_ = 0;  // reset retry_count if success
  return 0;
}

/*
** config loaders
** find and store proper server and location config to member variables
*/

// find and store proper server and location config to member variables
void Session::setupServerAndLocationConfig() {
  server_config_ = findServer();
  if (server_config_ == NULL) {
    createErrorResponse(HTTP_400);
    return;
  }
  location_config_ = findLocation();
}

// util of findServer
static bool isServerNameMatch(const std::string& host_header,
                              const std::string& server_name) {
  size_t pos_colon = host_header.find(':');
  if (pos_colon != std::string::npos) {
    // ignore after ':'
    return !host_header.compare(0, pos_colon, server_name);
  }
  return host_header == server_name;
}

// find matching server directive to request
const ServerConfig* Session::findServer() {
  // get ip and port
  sockaddr_in addr;
  socklen_t addrlen = sizeof(sockaddr_in);
#ifndef UNIT_TEST
  getsockname(sock_fd_, reinterpret_cast<struct sockaddr*>(&addr), &addrlen);
  ip_ = addr.sin_addr.s_addr;
  port_ = addr.sin_port;
#else
  // just for unit_test
  (void)addr;
  (void)addrlen;
  ip_ = 0x12345678;
  port_ = 0x1234;
#endif /* UNIT_TEST */

  // iterate for all server directive in main_config
  std::list<ServerConfig>::const_iterator itr_server;
  std::list<ServerConfig>::const_iterator end_server =
      main_config_.getServers().end();
  std::list<ServerConfig>::const_iterator itr_server_matched = end_server;
  for (itr_server = main_config_.getServers().begin(); itr_server != end_server;
       ++itr_server) {
    // check port and host
    std::list<std::pair<in_addr_t, uint16_t> >::const_iterator itr_listen;
    std::list<std::pair<in_addr_t, uint16_t> >::const_iterator end_listen =
        itr_server->getListen().end();
    bool flg_matched = false;
    for (itr_listen = itr_server->getListen().begin(); itr_listen != end_listen;
         ++itr_listen) {
      if ((itr_listen->first == INADDR_ANY || itr_listen->first == ip_) &&
          itr_listen->second == port_) {
        flg_matched = true;
        break;
      }
    }

    if (flg_matched) {
      // check server_name
      std::list<std::string>::const_iterator itr_sn;
      std::list<std::string>::const_iterator end_sn =
          itr_server->getServerName().end();
      for (itr_sn = itr_server->getServerName().begin(); itr_sn != end_sn;
           ++itr_sn) {
        std::map<std::string, std::string>::const_iterator itr_host =
            request_.getHeaders().find("host");
        // return if server name matched
        if (itr_host != request_.getHeaders().end() &&
            isServerNameMatch(itr_host->second, *itr_sn)) {
          return &(*itr_server);
        }
      }

      // save first server and go next
      if (itr_server_matched == end_server) {
        itr_server_matched = itr_server;
      }
    }
  }

  // return first server if no server name matched to request host header
  if (itr_server_matched == end_server) {
    return NULL;
  } else {
    return &(*itr_server_matched);
  }
}

// util of findLocation
bool Session::isLocationMatch(const std::string& loc_route,
                              const std::string& uri_path) const {
  return (!uri_path.compare(0, loc_route.length(), loc_route) &&
          (loc_route.back() == '/' || uri_path[loc_route.length()] == '/' ||
           uri_path.c_str()[loc_route.length()] == '\0'));
}

// find matching location config to request
const LocationConfig* Session::findLocation() const {
  // cannot find location if no server_config
  if (server_config_ == NULL) {
    return NULL;
  }

  // find location config best match
  const LocationConfig* location_config = NULL;
  std::list<LocationConfig>::const_iterator itr_loc;
  std::list<LocationConfig>::const_iterator end_loc =
      server_config_->getLocations().end();
  for (itr_loc = server_config_->getLocations().begin(); itr_loc != end_loc;
       ++itr_loc) {
    if (isLocationMatch(itr_loc->getRoute(), request_.getUri())) {
      // use route longer if more than one location route match
      if (location_config == NULL ||
          location_config->getRoute().length() < itr_loc->getRoute().length()) {
        location_config = &(*itr_loc);
      }
    }
  }
  return location_config;
}

/*
** File Readers
*/

// find and open file and create response header
void Session::startReadingFromFile(const std::string& filepath) {
  // openfile
  file_fd_ = open(filepath.c_str(), O_RDONLY);  // toriaezu
  if (file_fd_ == -1) {
    createErrorResponse(HTTP_403);
    return;
  }

  // set to non block
  fcntl(file_fd_, F_SETFL, O_NONBLOCK);
  if (file_fd_ == -1) {
    close(file_fd_);
    createErrorResponse(HTTP_503);
    return;
  }

  // create response header
  addResponseHeaderOfFile(filepath);  // add response header
  status_ = SESSION_FOR_FILE_READ;
}

void Session::addResponseHeaderOfFile(const std::string& filepath) {
  (void)filepath;
  response_.createStatusLine(HTTP_200);
  response_.addHeader("Content-Type", "text/html");
}

// find requested file
std::string Session::findFile(const std::string& uri) const {
  struct stat pathstat;
  std::string rootpath = findRoot();
  std::string filepath = rootpath + uri;

  if (stat(filepath.c_str(), &pathstat) == -1) {
    return "";  // no file or error
  }

  if (S_ISREG(pathstat.st_mode)) {
    return filepath;  // file found
  } else if (S_ISDIR(pathstat.st_mode)) {
    std::string res = findFileFromDir(filepath);
    return res.empty() ? "" : filepath + res;  // find from directive as "index"
  } else {
    return "";  // treat as file not found
  }
}

// find root config (called from findFile())
const std::string& Session::findRoot() const {
  // TODO: need to check location but later
  if (location_config_ != NULL && !location_config_->getRoot().empty()) {
    return location_config_->getRoot();
  }
  if (server_config_ != NULL && !server_config_->getRoot().empty()) {
    return server_config_->getRoot();
  }
  return main_config_.getRoot();
}

// find file as index directive (called from findFile())
std::string Session::findFileFromDir(const std::string& dirpath) const {
  // open directory to seek index file
  DIR* dir = opendir(dirpath.c_str());
  if (dir == NULL) {
    return "";
  }

  // check each file
  struct dirent* dent;
  while ((dent = readdir(dir))) {
    struct stat filestat;
    if (stat((dirpath + dent->d_name).c_str(), &filestat) == -1) {
      return "";
    }
    // case found
    if (S_ISREG(filestat.st_mode) && isIndex(dent->d_name)) {
      return dent->d_name;
    }
  }
  closedir(dir);

  // file not found
  return "";
}

// find check the filename is set by index directive
bool Session::isIndex(const std::string& filename) const {
  std::list<std::string>::const_iterator itr;
  std::list<std::string>::const_iterator itr_end;

  // find from index list of main_config_
  itr_end = main_config_.getIndex().end();
  for (itr = main_config_.getIndex().begin(); itr != itr_end; ++itr) {
    if (*itr == filename) {
      return true;
    }
  }

  // find from index list of server_config_
  if (server_config_ == NULL) {
    return false;
  }
  itr_end = server_config_->getIndex().end();
  for (itr = server_config_->getIndex().begin(); itr != itr_end; ++itr) {
    if (*itr == filename) {
      return true;
    }
  }

  // find from index list of location_config_
  if (location_config_ == NULL) {
    return false;
  }
  itr_end = location_config_->getIndex().end();
  for (itr = location_config_->getIndex().begin(); itr != itr_end; ++itr) {
    if (*itr == filename) {
      return true;
    }
  }

  // not found
  return false;
}

// read from filefd_ and store to response buf
int Session::readFromFile() {
  ssize_t n;
  char read_buf[BUFFER_SIZE];

  // read from file
  n = read(file_fd_, read_buf, BUFFER_SIZE);

  // retry seveal times even if read failed
  if (n == -1) {
    if (retry_count_ == RETRY_TIME_MAX) {
      retry_count_ = 0;

      // close file and make error responce
      close(file_fd_);
      createErrorResponse(HTTP_503);
      status_ = SESSION_FOR_CLIENT_SEND;
      return 0;
    }
    retry_count_++;
    return 0;
  }

  // reset retry conunt on success
  retry_count_ = 0;

  // if read all data, close file and create response header
  if (n == 0) {
    close(file_fd_);                    // close
    status_ = SESSION_FOR_CLIENT_SEND;  // set for send response
    return 0;
  }

  // append data to response
  response_.appendToBody(read_buf, n);

  return 0;
}

/*
** directory listing creators (todo!!)
*/

// struct to store info of each file
struct FileInfo {
  struct stat st;
  struct dirent dent;
};

// compare func to sort list of files
static bool compFiles(const struct FileInfo& a, const struct FileInfo& b) {
  // directory first
  if (S_ISDIR(a.st.st_mode) && !S_ISDIR(b.st.st_mode)) {
    return true;
  } else if (!S_ISDIR(a.st.st_mode) && S_ISDIR(b.st.st_mode)) {
    return false;
  }

  // modified time (don't care nano time)
  if (a.st.st_mtimespec.tv_sec < b.st.st_mtimespec.tv_sec) {
    return true;
  } else if (a.st.st_mtimespec.tv_sec > b.st.st_mtimespec.tv_sec) {
    return false;
  }

  // file name
  if (ft_strncmp(a.dent.d_name, b.dent.d_name, a.dent.d_namlen) < 0) {
    return true;
  }
  return false;
}

bool Session::isAutoIndexOn() const {
  return ((location_config_ && location_config_->getAutoIndex()) ||
          (server_config_ && server_config_->getAutoIndex()) ||
          main_config_.getAutoIndex());
}

void Session::createDirectoryListingHeader() {
  response_.appendToBody("<html>\n<head><title>Index of ");
  response_.appendToBody(request_.getUri());
  response_.appendToBody("</title></head>\n");
}

void Session::createDirectoryList(const std::list<struct FileInfo>& files) {
  response_.appendToBody(
      "<pre>\n<a href=\"../\">../</a>\n");  // .. always with you

  for (std::list<struct FileInfo>::const_iterator itr = files.begin();
       itr != files.end(); ++itr) {
    // get filename
    std::string filename(itr->dent.d_name);
    if (S_ISDIR(itr->st.st_mode)) {
      filename.append("/");
    }

    // create link
    response_.appendToBody("<a href=\"");
    response_.appendToBody(request_.getUri());
    if (request_.getUri().empty() ||
        request_.getUri()[request_.getUri().size() - 1] != '/') {
      response_.appendToBody("/");
    }
    response_.appendToBody(filename);
    response_.appendToBody("\">");
    response_.appendToBody(filename);
    response_.appendToBody("</a>");

    response_.appendToBody("                                        ");

    char buf[128];
    size_t ts_len =
        getTimeStamp(buf, 128, "%d-%b-%Y %k:%M", itr->st.st_mtimespec.tv_sec);
    response_.appendToBody(buf, ts_len);

    response_.appendToBody("                          ");

    // file size
    if (S_ISDIR(itr->st.st_mode)) {
      response_.appendToBody("-\n");
    } else {
      response_.appendToBody(std::to_string(itr->st.st_size));
      response_.appendToBody("\n");
    }
  }

  // end of list
  response_.appendToBody("</pre><hr></body>\n</html>\n");
}

void Session::createDirectoryListingBody(
    const std::list<struct FileInfo>& files) {
  // html header
  createDirectoryListingHeader();

  // create title
  response_.appendToBody("<body bgcolor=\"white\">\n");
  response_.appendToBody("<h1>Index of ");
  response_.appendToBody(request_.getUri());
  response_.appendToBody("</h1>\n<hr>\n");

  // create directory list
  createDirectoryList(files);
}

int Session::createFileList(const std::string& dirpath,
                            std::list<struct FileInfo>* files) const {
  // open directory
  DIR* dptr = opendir(dirpath.c_str());
  if (dptr == NULL) {
    return -1;
  }

  // read directory and make list of files
  struct FileInfo finfo;
  struct dirent* dent;
  while ((dent = readdir(dptr)) != NULL) {
    // ignore hidden data
    if (*(dent->d_name) == '.') {
      continue;
    }

    // get file stat
    if (lstat((dirpath + "/" + dent->d_name).c_str(), &finfo.st) == -1) {
      closedir(dptr);
      return -1;
    }

    finfo.dent = *dent;
    files->push_back(finfo);
  }
  closedir(dptr);

  // sort list of files
  files->sort(compFiles);
  return 0;
}

// create html for directory listing
// TODO: TIME HEADER
void Session::startDirectoryListing(const std::string& dirpath) {
  std::list<struct FileInfo> files;

  // check autoindex is on
  if (!isAutoIndexOn()) {
    createErrorResponse(HTTP_403);
    return;
  }

  if (createFileList(dirpath, &files) < 0) {
    createErrorResponse(HTTP_500);
    return;
  }

  // add header
  response_.createStatusLine(HTTP_200);
  response_.addHeader("Content-Type", "text/html");
  // response_.addHeader("Date", timestamp);  // add after merged

  /*** create body ***/
  createDirectoryListingBody(files);  // html body

  // to send response
  status_ = SESSION_FOR_CLIENT_SEND;
}

/*
** cgi handlers (TODO!!)
*/

std::string Session::findCgiPathFromUri() const {
  size_t pos;
  std::string cgipath;
  std::list<std::string>::const_iterator itr;
  std::list<std::string>::const_iterator end;

  // to check uri like "/hoge/fuga/fuga.cgi"
  std::string uri = request_.getUri() + "/";

  // find from location config
  if (location_config_ && !location_config_->getCgiExtension().empty()) {
    end = location_config_->getCgiExtension().end();
    for (itr = location_config_->getCgiExtension().begin(); itr != end; ++itr) {
      if ((pos = uri.find("." + *itr + "/")) != std::string::npos) {
        return uri.substr(0, pos + itr->length() + 1);
      }
    }
  }

  // find from server config
  if (server_config_ && !server_config_->getCgiExtension().empty()) {
    end = server_config_->getCgiExtension().end();
    for (itr = server_config_->getCgiExtension().begin(); itr != end; ++itr) {
      if ((pos = uri.find("." + *itr + "/")) != std::string::npos) {
        return uri.substr(0, pos + itr->length() + 1);
      }
    }
  }

  // find from main config
  if (!main_config_.getCgiExtension().empty()) {
    end = main_config_.getCgiExtension().end();
    for (itr = main_config_.getCgiExtension().begin(); itr != end; ++itr) {
      if ((pos = uri.find("." + *itr + "/")) != std::string::npos) {
        return uri.substr(0, pos + itr->length() + 1);
      }
    }
  }

  return "";
}

bool Session::isCgiFile(const std::string& filepath) const {
  size_t pos;
  std::list<std::string>::const_iterator itr;
  std::list<std::string>::const_iterator end;

  // find from location config
  if (location_config_ && !location_config_->getCgiExtension().empty()) {
    end = location_config_->getCgiExtension().end();
    for (itr = location_config_->getCgiExtension().begin(); itr != end; ++itr) {
      if ((pos = filepath.find("." + *itr)) != std::string::npos &&
          (pos + itr->length() + 1 == filepath.length())) {
        return true;
      }
    }
  }

  // find from server config
  if (server_config_ && !server_config_->getCgiExtension().empty()) {
    end = server_config_->getCgiExtension().end();
    for (itr = server_config_->getCgiExtension().begin(); itr != end; ++itr) {
      if ((pos = filepath.find("." + *itr)) != std::string::npos &&
          (pos + itr->length() + 1 == filepath.length())) {
        return true;
      }
    }
  }

  // find from main config
  if (!main_config_.getCgiExtension().empty()) {
    end = main_config_.getCgiExtension().end();
    for (itr = main_config_.getCgiExtension().begin(); itr != end; ++itr) {
      if ((pos = filepath.find("." + *itr)) != std::string::npos &&
          (pos + itr->length() + 1 == filepath.length())) {
        return true;
      }
    }
  }

  return false;
}

void Session::createCgiProcess(const std::string& filepath,
                               const std::string& cgiuri) {
  // prepare argvs and env vars here and pass them to cgiHandler
  CgiParams cgi_params(*this);
  char** argv = cgi_params.storeArgv(filepath, cgiuri, request_);
  char** meta_variables = cgi_params.storeMetaVariables(cgiuri, request_);

  HTTPStatusCode http_status =
      cgi_handler_.createCgiProcess(filepath, argv, meta_variables);

  if (http_status != HTTP_200) {
    std::cout << "[error] failed to create cgi process" << std::endl;
    createErrorResponse(http_status);
  }

  // response_.createStatusLine(HTTP_200); cgi scripts can produce status by
  // themselves

  if (request_.getMethod() == "POST" || request_.getMethod() == "PUT") {
    status_ = SESSION_FOR_CGI_WRITE;
    return;
  }
  close(cgi_handler_.getInputFd());
  status_ = SESSION_FOR_CGI_READ;
}

// write to cgi process (TODO!!)
int Session::writeToCgi() {
  ssize_t n;

  // write to cgi process
  n = cgi_handler_.writeToCgi(&(request_.getBody()[0]),
                              request_.getBody().size());

  // retry several times even if write failed
  if (n == -1) {
    std::cout << "[error] failed to write to CGI process" << std::endl;

    // give up if reached retry count to maximum
    if (retry_count_ == RETRY_TIME_MAX) {
      retry_count_ = 0;

      // close connection
      std::cout << "[error] close connection from CGI process" << std::endl;
      close(cgi_handler_.getInputFd());

      // expect response from cgi process
      status_ = SESSION_FOR_CGI_READ;  // to read from cgi process
      return 0;
    }

    retry_count_++;
    return 0;
  }

  // reset retry conunt on success
  retry_count_ = 0;

  // erase written data
  request_.eraseBody(n);

  // written all data
  if (request_.getBody().empty()) {
    close(cgi_handler_.getInputFd());
    status_ = SESSION_FOR_CGI_READ;  // to read from cgi process
    return 0;
  }

  // to next read
  return 0;
}

// read from to cgi process (TODO!!)
int Session::readFromCgi() {
  ssize_t n;

  // read from cgi process
  n = cgi_handler_.readFromCgi();
  // retry seveal times even if read failed
  if (n == -1) {
    std::cout << "[error] failed to read from cgi process" << std::endl;
    if (retry_count_ == RETRY_TIME_MAX) {
      retry_count_ = 0;

      // close connection and make error responce
      std::cout << "[error] close connection to CGI process" << std::endl;
      close(cgi_handler_.getOutputFd());
      // response_buf_ = "500 internal server error";  // TODO: make response
      // func

      // kill the process on error (if failed kill, we can do nothing...)
      if (kill(cgi_handler_.getPid(), SIGKILL) == -1) {
        std::cout << "[error] failed kill cgi process" << std::endl;
      }

      // to send error response to client
      status_ = SESSION_FOR_CLIENT_SEND;
      return 0;
    }
    retry_count_++;
    return 0;
  }

  // reset retry conunt on success
  retry_count_ = 0;

  // check if pipe closed
  if (n == 0) {
    // append data to response
    ssize_t end_header =
        parseReadBuf(&(cgi_handler_.getBuf())[0], cgi_handler_.getBuf().size());
    if (end_header == -1) {
      createErrorResponse(HTTP_502);  // Bad Gateway but does not close session
      return 0;
    }
    response_.appendToBody(&cgi_handler_.getBuf()[0] + end_header + 1,
                           cgi_handler_.getBuf().size() - (end_header + 1));
    close(cgi_handler_.getOutputFd());  // close pipefd
    status_ = SESSION_FOR_CLIENT_SEND;  // set for send response
    return 0;
  }
  return 0;
}

/*
** parse read buf from cgi script
** if there is content-type header, set header
*/

ssize_t Session::parseReadBuf(const char* read_buf, ssize_t n) {
  std::map<std::string, std::string> header;
  ssize_t i = 0;
  ssize_t ret = -1;
  ssize_t begin = 0;
  while (i < n) {
    begin = i;
    while (read_buf[i] > 31 && read_buf[i] != 127 &&
           ft_strchr("()<>@,;:\\\"/[]?={} \t", read_buf[i]) == NULL) {
      ++i;
    }
    if (read_buf[i] != ':') {  // no space permitted between header key and :
      return -1;
    }
    std::string key = std::string(&read_buf[begin], &read_buf[i]);
    ++i;
    while (read_buf[i] == ' ') {  // TAB等含むかよくわからず(RFCから読み取れず)
      ++i;
    }
    begin = i;
    while (read_buf[i] > 31 && read_buf[i] != 127) {
      ++i;
    }
    header[key] = std::string(&read_buf[begin], &read_buf[i]);
    if (!ft_strncmp(read_buf + i, "\n\n", 2)) {
      ret = 1;
    } else if (!ft_strncmp(read_buf + i, "\r\n\r\n", 4)) {
      ret = 3;
    }
    if (ret > 0) {
      if (i == 0) {  // no header provided
        return -1;
      }
      break;  // go outside of while loop to  check content of header
    }
    ++i;
  }
  /* header must include at least one of Content-type, Location or Status*/
  std::map<std::string, std::string>::iterator status_itr =
      header.find("Status");
  if (header.find("Content-Type") != header.end() ||
      header.find("Location") != header.end() || status_itr != header.end()) {
    if (status_itr != header.end()) {  // status code created in cgi script
      response_.createStatusLine(status_itr->second);
    } else {  // No status code created in cgi script
      response_.createStatusLine(HTTP_200);
    }
    for (std::map<std::string, std::string>::iterator itr = header.begin();
         itr != header.end(); ++itr) {
      if (itr == status_itr) {  // skip key of status
        continue;
      }
      response_.addHeader(itr->first, itr->second);  // add header
    }
    return i + ret;  // Parse OK then return the pos of end of header
  }
  return -1;  // find No valid header
}

/*
** file writers
*/

void Session::startWritingToFile() {
  // check if request uri upload path
  std::string upload_store =
      findUploadStore(request_.getUri());  // relative path from root
  if (upload_store.empty()) {
    createErrorResponse(HTTP_405);
    return;
  }

  // create and append filename to upload store
  if (*(upload_store.end() - 1) != '/') {
    upload_store.push_back('/');  // append "/" if missing
  }
  upload_store.append(createFilename());  // append filename

  // create filepath
  std::string filepath = findRoot();
  if (*(filepath.end() - 1) != '/' || upload_store[0] != '/') {
    filepath.append("/");  // append "/" if missing
  }
  filepath.append(upload_store);

  // create response header
  response_.createStatusLine(HTTP_201);
  response_.addHeader("Location", upload_store);

  // open file
  file_fd_ = open(filepath.c_str(), O_RDWR | O_CREAT, 0644);  // toriaezu
  if (file_fd_ == -1) {
    createErrorResponse(HTTP_500);
    status_ = SESSION_FOR_CLIENT_SEND;
    return;
  }

  // set as non block
  if (fcntl(file_fd_, F_SETFL, O_NONBLOCK) == -1) {
    createErrorResponse(HTTP_500);
    status_ = SESSION_FOR_CLIENT_SEND;
    return;
  }

  status_ = SESSION_FOR_FILE_WRITE;
}

std::string Session::findUploadStore(const std::string& uri) const {
  if (location_config_ != NULL &&
      isLocationMatch(location_config_->getUploadPass(), uri)) {
    return location_config_->getUploadStore();
  }
  std::cout << server_config_->getUploadPass() << std::endl;
  if (server_config_ != NULL &&
      isLocationMatch(server_config_->getUploadPass(), uri)) {
    return server_config_->getUploadStore();
  }
  return "";
}

std::string Session::createFilename() const {
  std::string filename = "file";  // basename (tmp)
  char buf[512];

  getTimeStamp(buf, 512, "%y%m%d%H%M%S");
  return std::string("file") + buf + "_" + std::to_string(rand()) +
         getFileExtension();
}

// get file extension from mime type
std::string Session::getFileExtension() const {
  // find content-type header
  std::map<std::string, std::string>::const_iterator itr =
      request_.getHeaders().find("content-type");
  if (itr == request_.getHeaders().end()) {
    return "";  // return empty strin
  }

  // get mime type from header
  std::string mime_type;
  size_t posend = std::min(itr->second.find(' '), itr->second.find(';'));
  if (posend == std::string::npos) {
    mime_type = itr->second;
  } else {
    mime_type = itr->second.substr(0, posend);
  }

  // get file extension
  itr = map_mime_ext_.find(mime_type);
  if (itr == map_mime_ext_.end()) {
    return "";  // no mime type matched
  }
  return "." + itr->second;  // return extention found
}

int Session::writeToFile() {
  ssize_t n;

  // write to file
  n = write(file_fd_, &(request_.getBody()[0]), request_.getBody().size());
  // retry several times even if write failed
  if (n == -1) {
    std::cout << "[error] failed to write to file" << std::endl;

    // give up if reached retry count to maximum
    if (retry_count_ == RETRY_TIME_MAX) {
      close(file_fd_);
      retry_count_ = 0;

      // close connection
      std::cout << "[error] close file" << std::endl;
      createErrorResponse(HTTP_500);
      status_ = SESSION_FOR_CLIENT_SEND;  // to send response to client
      return 0;
    }

    retry_count_++;
    return 0;
  }

  // reset retry conunt on success
  retry_count_ = 0;

  // erase written data
  request_.eraseBody(n);

  // written all data
  if (request_.getBody().empty()) {
    close(file_fd_);

    // create response to notify the client
    // response_buf_ = "201 created";
    status_ = SESSION_FOR_CLIENT_SEND;  // to send response to client
  }
  // to next read
  return 0;
}

Session::CgiParams::CgiParams(Session const& session)
    : session_(session), argv_(NULL), envp_(NULL) {}

Session::CgiParams::~CgiParams() {
  if (argv_ != NULL) {
    for (int i = 0; argv_[i] != NULL; ++i) {
      free(argv_[i]);
    }
    free(argv_);
  }
  if (envp_ != NULL) {
    for (int i = 0; envp_[i] != NULL; ++i) {
      free(envp_[i]);
    }
    free(envp_);
  }
}

char** Session::CgiParams::storeArgv(const std::string& filepath,
                                     const std::string& cgiuri,
                                     const Request& request) {
  std::string argv = filepath;
  /* in case of XXX.cgi?argv1+argv2, if there is "=" in query string, "+"
   * becomes literal not delimitter */
  if (request.getQuery().find("=") == std::string::npos &&
      request.getQuery().find("+") != std::string::npos) {
    argv += "+";
    argv += request.getQuery();
    argv_ = ft_split(argv.c_str(), '+');
    if (!argv_) {
      throw std::bad_alloc();
    }
    return argv_;
  }
  /* in case of /cgi-bin/XXX.cgi/argv1/argv2 */
  argv += session_.getPathInfo(cgiuri);
  argv_ = ft_split(argv.c_str(), '/');
  if (!argv_) {
    throw std::bad_alloc();
  }
  int begin = 0;
  for (int i = 0; argv_[i] != NULL; ++i) {
    if (ft_strnstr(argv_[i], ".cgi", ft_strlen(argv_[i])) != NULL) {
      free(argv_[i]);
      argv_[i] = ft_strdup(filepath.c_str());
      if (!argv_[i]) {
        throw std::bad_alloc();
      }
      begin = i;
      break;
    }
  }
  return argv_ + begin;
}

char** Session::CgiParams::storeMetaVariables(const std::string& cgiuri,
                                              const Request& request) {
  std::vector<std::string> meta_variables;
  std::string tmp;
  const std::map<std::string, std::string>& headers = request.getHeaders();
  tmp = "AUTH_TYPE=";
  tmp += session_.getFromHeaders(headers, "authorization");  // TBC!
  meta_variables.push_back(tmp);
  tmp = "CONTENT_LENGTH=";
  tmp += session_.getFromHeaders(headers, "content-length");
  meta_variables.push_back(tmp);
  tmp = "CONTENT_TYPE=";
  tmp += session_.getFromHeaders(headers, "content-type");
  meta_variables.push_back(tmp);
  tmp = "GATEWAY_INTERFACE=";  //プロトコル名称入れてもらう？
  tmp += session_.getFromHeaders(headers, "gateway-interface");  //?
  meta_variables.push_back(tmp);
  tmp = "PATH_INFO=";  // testerで求める挙動は違うようだ（discord #webserv)
  tmp += session_.getPathInfo(cgiuri);
  meta_variables.push_back(tmp);
  tmp = "PATH_TRANSLATED=";  // Document root + PATH_INFO if there is PATH_INFO
  if (session_.getPathInfo(cgiuri) != "") {
    tmp += session_.findRoot();
    tmp += session_.getPathInfo(cgiuri);
  }
  meta_variables.push_back(tmp);
  tmp = "QUERY_STRING=";
  tmp += request.getQuery();
  meta_variables.push_back(tmp);
  tmp = "REMOTE_ADDR=";
  tmp += getIpAddress(session_.getIp());
  meta_variables.push_back(tmp);
  tmp = "REMOTE_IDENT=";
  tmp += "TEST";  // TBC!
  meta_variables.push_back(tmp);
  tmp = "REMOTE_USER=";
  tmp += "TEST";  // TBC!
  meta_variables.push_back(tmp);
  tmp = "REQUEST_METHOD=";
  tmp += request.getMethod();
  meta_variables.push_back(tmp);
  tmp = "REQUEST_URI=";
  tmp += request.getUri();
  meta_variables.push_back(tmp);
  tmp = "SCRIPT_NAME=";
  tmp += cgiuri;
  meta_variables.push_back(tmp);
  tmp = "SERVER_NAME=";
  tmp += session_.getFromHeaders(headers, "host");
  meta_variables.push_back(tmp);
  tmp = "SERVER_PORT=";
  char* server_port = ft_itoa(ft_htons(session_.getPort()));
  if (!server_port) {
    return NULL;
  }
  tmp += server_port;
  free(server_port);
  meta_variables.push_back(tmp);
  tmp = "SERVER_PROTOCOL=HTTP/1.1";
  meta_variables.push_back(tmp);
  tmp = "SERVER_SOFTWARE=";
  tmp += SOFTWARE_NAME;
  meta_variables.push_back(tmp);
  return vecToChar(meta_variables);
}

std::string Session::getFromHeaders(
    const std::map<std::string, std::string>& headers,
    const std::string key) const {
  std::map<std::string, std::string>::const_iterator itr = headers.find(key);
  if (itr == headers.end()) {
    return "";
  }
  return (*itr).second;
}

std::string Session::getPathInfo(const std::string& cgiuri) const {
  std::string path_info = request_.getUri();
  path_info.erase(0, cgiuri.length());
  return path_info;
}

char** Session::CgiParams::vecToChar(std::vector<std::string>& meta_variables) {
  envp_ = (char**)malloc(sizeof(char*) * (meta_variables.size() + 1));
  if (!envp_) {
    throw std::bad_alloc();
  }
  for (size_t i = 0; i < meta_variables.size(); ++i) {
    envp_[i] = ft_strdup(meta_variables[i].c_str());
    if (!envp_[i]) {
      throw std::bad_alloc();
    }
  }
  envp_[meta_variables.size()] = NULL;
  return envp_;
}

/*
** initMapMimeExt
**
** initialize maps MIME TYPE (to/from) file extension.
** covering common mime types listed in mdn docs below.
** https://developer.mozilla.org/ja/docs/Web/HTTP/Basics_of_HTTP/MIME_types/Common_types
*/

void Session::initMapMineExt() {
  static bool flg_set;
  if (flg_set) {
    return;
  }
  flg_set = true;

  // set extention to MIME
  map_ext_mime_["aac"] = "audio/aac";              // AAC 音声
  map_ext_mime_["abw"] = "application/x-abiword";  // AbiWord文書
  map_ext_mime_["arc"] =
      "application/x-freearc";  //(複数のファイルが埋め込まれた)アーカイブ文書
  map_ext_mime_["avi"] = "video/x-msvideo";  // AVI: Audio Video Interleave
  map_ext_mime_["azw"] =
      "application/vnd.amazon.ebook";  // Amazon Kindle eBook 形式
  map_ext_mime_["bin"] =
      "application/octet-stream";  //任意の種類のバイナリーデータ
  map_ext_mime_["bmp"] = "image/bmp";            // Windows OS/2
                                                 //ビットマップ画像
  map_ext_mime_["bz"] = "application/x-bzip";    // BZip アーカイブ
  map_ext_mime_["bz2"] = "application/x-bzip2";  // BZip2 アーカイブ
  map_ext_mime_["csh"] = "application/x-csh";    // C-Shell スクリプト
  map_ext_mime_["css"] = "text/css";  //カスケーディングスタイルシート
                                      //(CSS)
  map_ext_mime_["csv"] = "text/csv";            //カンマ区切り値 (CSV)
  map_ext_mime_["doc"] = "application/msword";  // Microsoft Word
  map_ext_mime_["docx"] =
      "application/"
      "vnd.openxmlformats-officedocument.wordprocessingml.document";  // Microsoft
                                                                      // Word
                                                                      // (OpenXML)
  map_ext_mime_["eot"] =
      "application/vnd.ms-fontobject";  // MS 埋め込み OpenTypeフォント
  map_ext_mime_["epub"] = "application/epub+zip";  //電子出版 (EPUB)
  map_ext_mime_["gz"] = "application/gzip";        // GZip 圧縮アーカイブ
  map_ext_mime_["gif"] = "image/gif";  //グラフィック交換形式 (GIF)
  map_ext_mime_["htm"] = "text/html";  //ハイパーテキストマークアップ言語
                                       //(HTML)
  map_ext_mime_["html"] =
      "text/html";  //ハイパーテキストマークアップ言語//(HTML)
  map_ext_mime_["ico"] = "image/vnd.microsoft.icon";  //アイコン形式
  map_ext_mime_["ics"] = "text/calendar";             // iCalendar 形式
  map_ext_mime_["jar"] = "application/java-archive";  // Java Archive (JAR)
  map_ext_mime_["jpeg"] = "image/jpeg";               // JPEG 画像
  map_ext_mime_["jpg"] = "image/jpeg";                // JPEG 画像
  map_ext_mime_["js"] = "text/javascript";            // JavaScript
  map_ext_mime_["json"] = "application/json";         // JSON 形式
  map_ext_mime_["jsonld"] = "application/ld+json";    // JSON-LD 形式
  map_ext_mime_["midi"] = "audio/x-midi";    // Musical Instrument Digital
                                             // Interface (MIDI)
  map_ext_mime_["mid"] = "audio/midi";       // Musical Instrument Digital
                                             // Interface (MIDI)
  map_ext_mime_["mjs"] = "text/javascript";  // JavaScript モジュール
  map_ext_mime_["mp3"] = "audio/mpeg";       // MP3 音声
  map_ext_mime_["mpeg"] = "video/mpeg";      // MPEG 動画
  map_ext_mime_["mpkg"] =
      "application/vnd.apple.installer+xml";  // Apple Installer Package
  map_ext_mime_["odp"] =
      "application/vnd.oasis.opendocument.presentation";  // OpenDocuemnt
                                                          //プレゼンテーション文書
  map_ext_mime_["ods"] =
      "application/vnd.oasis.opendocument.spreadsheet";  // OpenDocuemnt
                                                         //表計算文書
  map_ext_mime_["odt"] =
      "application/vnd.oasis.opendocument.text";  // OpenDocument テキスト文書
  map_ext_mime_["oga"] = "audio/ogg";             // OGG 音声
  map_ext_mime_["ogv"] = "video/ogg";             // OGG 動画
  map_ext_mime_["ogx"] = "application/ogg";       // OGG
  map_ext_mime_["opus"] = "audio/opus";           // Opus 音声
  map_ext_mime_["otf"] = "font/otf";              // OpenType フォント
  map_ext_mime_["png"] = "image/png";             // Portable Network Graphics
  map_ext_mime_["pdf"] =
      "application/pdf";  // Adobe Portable Document Format (PDF)
  map_ext_mime_["php"] = "application/x-httpd-php";  // Hypertext Preprocessor
                                                     // (Personal Home Page)
  map_ext_mime_["ppt"] =
      "application/vnd.ms-powerpoint";  // Microsoft PowerPoint
  map_ext_mime_["pptx"] =
      "application/"
      "vnd.openxmlformats-officedocument.presentationml.presentation";  // Microsoft
                                                                        // PowerPoint
                                                                        // (OpenXML)
  map_ext_mime_["rar"] = "application/vnd.rar";  // RAR アーカイブ
  map_ext_mime_["rtf"] = "application/rtf";  //リッチテキスト形式 (RTF)
  map_ext_mime_["sh"] = "application/x-sh";  // Bourne shell スクリプト
  map_ext_mime_["svg"] = "image/svg+xml";    // Scalable Vector Graphics (SVG)
  map_ext_mime_["swf"] =
      "application/x-shockwave-flash";  // Small web format (SWF) または Adobe
                                        // Flash 文書
  map_ext_mime_["tar"] = "application/x-tar";  // Tape Archive (TAR)
  map_ext_mime_["tif"] = "image/tiff";   // Tagged Image File Format (TIFF)
  map_ext_mime_["tiff"] = "image/tiff";  // Tagged Image File Format (TIFF)
  map_ext_mime_["ts"] = "video/mp2t";    // MPEG transport stream
  map_ext_mime_["ttf"] = "font/ttf";     // TrueType フォント
  map_ext_mime_["txt"] = "text/plain";   //テキストファイル (一般に
                                         // ASCII or ISO
                                         // 8859-<em>n</em>)
  map_ext_mime_["vsd"] = "application/vnd.visio";  // Microsoft Visio
  map_ext_mime_["wav"] = "audio/wav";              // Waveform 音声形式
  map_ext_mime_["weba"] = "audio/webm";            // WEBM 音声
  map_ext_mime_["webm"] = "video/webm";            // WEBM 動画
  map_ext_mime_["webp"] = "image/webp";            // WEBP 画像
  map_ext_mime_["woff"] = "font/woff";    // Web Open Font Format (WOFF)
  map_ext_mime_["woff2"] = "font/woff2";  // Web Open Font Format (WOFF)
  map_ext_mime_["xhtml"] = "application/xhtml+xml";   // XHTML
  map_ext_mime_["xls"] = "application/vnd.ms-excel";  // Microsoft Excel
  map_ext_mime_["xlsx"] =
      "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";  //    Microsoft Excel (OpenXML)
  map_ext_mime_["xml"] = "application/xml";  // XML
                                             //(一般のユーザから読める場合)
  map_ext_mime_["xml"] = "text/xml";  // XML
                                      //(一般のユーザから読めない場合)
  map_ext_mime_["xul"] = "application/vnd.mozilla.xul+xml";  // XUL
  map_ext_mime_["zip"] = "application/zip";  // ZIP アーカイブ
  map_ext_mime_["3gp"] = "audio/3gpp";  // 3GPP 音声/動画コンテナー (動画含まず)
  map_ext_mime_["3gp"] = "video/3gpp";  // 3GPP 音声/動画コンテナー
  map_ext_mime_["3g2"] =
      "audio/3gpp2";  // 3GPP2 音声/動画コンテナー (動画含まず)
  map_ext_mime_["3g2"] = "video/3gpp2";  // 3GPP2 音声/動画コンテナー
  map_ext_mime_["7z"] = "application/x-7z-compressed";  // 7-zipアーカイブ

  // set extention to MIME
  for (std::map<std::string, std::string>::const_iterator itr =
           map_ext_mime_.begin();
       itr != map_ext_mime_.end(); ++itr) {
    map_mime_ext_[itr->second] = itr->first;
  }
  map_mime_ext_["audio/3gpp"] = "3gp";
  map_mime_ext_["audio/3gpp2"] = "3g2";
}
