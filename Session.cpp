/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 23:21:37 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/04/11 22:02:10 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Session.hpp"

#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

#include "webserv_utils.hpp"

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
      location_config_(NULL) {}

Session::~Session(){};

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
*/

int Session::checkSelectedAndExecute(fd_set* rfds, fd_set* wfds) {
  if (status_ == SESSION_FOR_CLIENT_RECV && FD_ISSET(sock_fd_, rfds)) {
    if (receiveRequest() == -1) {
      return (-1);
    } else {
      std::cout << "[webserv] received request data" << std::endl;
      return (1);
    }
  } else if (status_ == SESSION_FOR_FILE_READ && FD_ISSET(file_fd_, rfds)) {
    if (readFromFile() == -1) {
      return (-1);
    } else {
      std::cout << "[webserv] read data from file" << std::endl;
      return (1);
    }
  } else if (status_ == SESSION_FOR_FILE_READ && FD_ISSET(file_fd_, wfds)) {
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
      std::cout << "[webserv] sent response data" << std::endl;
      return (-1);
    } else {
      return (1);
    }
  } else {
    return (0);
  }
}

/*
** PRIVATE FUNCTIONS
*/

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
  }
  retry_count_ = 0;
  return checkReceiveReturn(ret);
}

/*
** check return value of request_.receive(sock_fd_);
*/
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
  // case GET
  if ((request_.getMethod() == "GET" && isMethodAllowed(HTTP_GET)) ||
      (request_.getMethod() == "HEAD" && isMethodAllowed(HTTP_HEAD))) {
    startCreateResponseToGet();
    return;
  }

  // case POST (for now, just try to use cgi in startReadingFromFile)
  // if (request_.getMethod() == "POST" && !isMethodAllowed(HTTP_POST)) {
  // startReadingFromFile();  // header will checked in startReadingFromFile
  // return;
  // }

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
    close(sock_fd_);
    return 1;  // return 1 if all data sent (this session will be closed)
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
  in_addr_t ip = 0x12345678;
  uint16_t port = 0x1234;
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
std::string Session::findRoot() const {
  // TODO: need to check location but later
  if (server_config_->getRoot().empty()) {
    return main_config_.getRoot();
  } else {
    return server_config_->getRoot();
  }
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

void Session::startDirectoryListing(const std::string& filepath) {
  response_.createStatusLine(HTTP_200);
  response_.addHeader("Content-Type", "text/html");

  // [TEMP] response
  response_.appendToBody("<h1>autoindex not yet implemented: ", 35);
  response_.appendToBody(filepath.c_str(), filepath.length());
  response_.appendToBody("</h1>\n", 6);
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
  const std::vector<std::string> meta_variables = storeMetaVariables(cgiuri);
  HTTPStatusCode http_status =
      cgi_handler_.createCgiProcess(filepath, meta_variables);
  if (http_status != HTTP_200) {
    std::cout << "[error] failed to create cgi process" << std::endl;
    createErrorResponse(http_status);
  }

  // [TEMP] create response header
  response_.createStatusLine(HTTP_200);
  response_.addHeader("Content-Type", "text/html");

  // [TEMP] response
  (void)cgiuri;
  response_.appendToBody("<h1>cgi not yet implemented: ", 29);
  response_.appendToBody(filepath.c_str(), filepath.length());
  response_.appendToBody("</h1>\n", 6);
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
  n = cgi_handler_.writeToCgi(&(request_.getBuf()[0]),
                              request_.getBuf().size());

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
  request_.eraseBuf(n);

  // written all data
  if (request_.getBuf().empty()) {
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
  char read_buf[BUFFER_SIZE];

  // read from cgi process
  n = cgi_handler_.readFromCgi(read_buf, BUFFER_SIZE);
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
    close(cgi_handler_.getOutputFd());  // close pipefd
    status_ = SESSION_FOR_CLIENT_SEND;  // set for send response
    return 0;
  }

  // append data to response
  response_.appendToBody(read_buf, n);

  return 0;
}

/*
** file writers (TODO!!)
*/

void Session::startWritingToFile() {
  file_fd_ = open("./test_req.txt", O_RDWR | O_CREAT, 0644);  // toriaezu
  if (file_fd_ == -1) {
    createErrorResponse(HTTP_404);
    status_ = SESSION_FOR_CLIENT_SEND;
  }
  fcntl(file_fd_, F_SETFL, O_NONBLOCK);
  status_ = SESSION_FOR_FILE_WRITE;
}

int Session::writeToFile() {
  ssize_t n;

  // write to file
  n = write(file_fd_, &(request_.getBuf()[0]), request_.getBuf().size());
  // retry several times even if write failed
  if (n == -1) {
    std::cout << "[error] failed to write to file" << std::endl;

    // give up if reached retry count to maximum
    if (retry_count_ == RETRY_TIME_MAX) {
      retry_count_ = 0;

      // close connection
      std::cout << "[error] close file" << std::endl;
      // close(file_fd_);

      // send response to notify request failed
      //  response_buf_ = "500 server error"; /* (tmp setter) error msg
      //  generator in Request class*/
      status_ = SESSION_FOR_CLIENT_SEND;  // to send response to client
      return 0;
    }

    retry_count_++;
    return 0;
  }

  // reset retry conunt on success
  retry_count_ = 0;

  // erase written data
  request_.eraseBuf(n);

  // written all data
  if (request_.getBuf().empty()) {
    close(file_fd_);

    // create response to notify the client
    // response_buf_ = "201 created";
    status_ = SESSION_FOR_CLIENT_SEND;  // to send response to client
    return 0;
  }
  // to next read
  return 0;
}

const std::vector<std::string> Session::storeMetaVariables(const std::string& cgiuri) {
  std::vector<std::string> envp;
  std::string tmp;
  const std::map<std::string, std::string>& headers = request_.getHeaders();
  tmp = "AUTH_TYPE=";
  tmp += getFromHeaders(headers, "authorization");
  envp.push_back(tmp);
  tmp = "CONTENT_LENGTH=";
  tmp += getFromHeaders(headers, "content-length");  // if  not find "";
  envp.push_back(tmp);
  tmp = "CONTENT_TYPE=";  // how to get content-type??? 入れてもらう！
  tmp += getFromHeaders(headers, "content-type");
  envp.push_back(tmp);
  tmp = "GATEWAY_INTERFACE=";  //プロトコル名称入れてもらう？
  tmp += getFromHeaders(headers, "gateway-interface");
  envp.push_back(tmp);
  tmp = "PATH_INFO=";//testerで求める挙動は違うようだ（discord #webserv)
  std::string path_info = getPathInfo(cgiuri);
  tmp += path_info;
  envp.push_back(tmp);
  tmp = "PATH_TRANSLATED=";  // Document root + PATH_INFO
  tmp += findRoot();
  tmp += path_info;
  envp.push_back(tmp);
  tmp =
      "QUERY_STRING=";  //?test.cgi?a=b&c=d
                        //->parseするのはcgiの責任parseしたのから再構築してもいい、直してもいい
  tmp += request_.getQuery();
  envp.push_back(tmp);
  tmp = "REMOTE_ADDR=";
  tmp += "TEST";
  //(findServerでportをメンバとしてもらってしまう)
  envp.push_back(tmp);
  tmp = "REMOTE_IDENT=";
  tmp += "TEST";
  envp.push_back(tmp);
  tmp = "REMOTE_USER=";
  tmp += "TEST";
  envp.push_back(tmp);
  tmp = "REQUEST_METHOD=";
  tmp += request_.getMethod();
  std::cout << "Method: " << request_.getMethod() << std::endl;
  envp.push_back(tmp);
  tmp = "REQUEST_URI=";
  tmp += request_.getUri();
  envp.push_back(tmp);
  tmp = "SCRIPT_NAME=";  // cgi script名(cgi_uriの最後を使う)ft_splitで！
  tmp += "TEST";
  envp.push_back(tmp);
  tmp = "SERVER_NAME=";
  tmp += getFromHeaders(headers, "host");
  envp.push_back(tmp);
  tmp = "SERVER_PORT=";  // port (findServerでipをメンバとしてもらってしまう)
  tmp += "TEST";
  envp.push_back(tmp);
  tmp = "SERVER_PROTOCOL=HTTP/1.1";
  envp.push_back(tmp);
  tmp = "SERVER_SOFTWARE=";
  tmp += SOFTWARE_NAME;
  envp.push_back(tmp);
  return envp;
}

std::string Session::getFromHeaders(const std::map<std::string, std::string>& headers,
                            const std::string key) {
  std::map<std::string, std::string>::const_iterator itr = headers.find(key);
  if (itr == headers.end()) {
    return "";
  }
  return (*itr).second;
}

std::string Session::getPathInfo(const std::string& cgiuri) {
  std::string path_info = request_.getUri();
  path_info.erase(0, cgiuri.length());
  return path_info;
}
