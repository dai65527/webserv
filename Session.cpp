/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 23:21:37 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/27 23:15:56 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Session.hpp"

#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

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

  // receive returns...
  // -4:failed to receive, -3:http411 Length Required -2:http505, -1:http400, 0:received all, 1:continue
  ret = request_.receive(sock_fd_);
  if (ret == -4) {
    if (retry_count_ == RETRY_TIME_MAX) {
      // then try to send return internal server error
      createErrorResponse(HTTP_500);
    } else {
      retry_count_++;
    }
    return 0;
  }
  retry_count_ = 0;
  if (ret == -1 || ret == -2) {
    createErrorResponse(ret == -1 ? HTTP_400 : HTTP_505);
  } else if (ret == 0) {
    startCreateResponse();
  }
  return 0;  // continue to receive
}

/*
** response creation manager
*/

// check request, load proper config and start creating response
void Session::startCreateResponse() {
  // set server and location config according to request
  setupServerAndLocationConfig();

  // if error, will create error response and set status_ inside.
  switch (checkResponseType()) {
    // read from file
    case 0:
      startReadingFromFile();
      break;

    // write to file
    case 1:
      startDirectoryListing();
      break;

    // write to file
    case 2:
      startWritingToFile();
      break;

    // create cgi process
    case 3:
      startCgiProcess();
      break;
  }
}

// check request and config and decide response type (set status_)
// not yet implimented enough (now is for test)
int Session::checkResponseType() {
  // if (!request_.getBuf().compare(0, 4, "read", 0, 4)) {
  //   return 0;
  // } else if (!request_.getBuf().compare(0, 5, "write", 0, 5)) {
  //   return 1;
  // } else if (!request_.getBuf().compare(0, 3, "cgi", 0, 3)) {
  //   return 2;
  // }

  // check method
  if (request_.getMethod() == "GET" /* need to check avaliavlity */) {
    return 0;
  } else {
    createErrorResponse(HTTP_505);
  }

  status_ = SESSION_FOR_CLIENT_SEND;
  return -1;
}

// create error response message
// yet not enough
void Session::createErrorResponse(HTTPStatusCode http_status) {
  // TODO: ちゃんとする
  response_.appendRawData("error: ", 7);
  response_.appendRawData(std::to_string(http_status).c_str(), 3);
  status_ = SESSION_FOR_CLIENT_SEND;
}

// send response
int Session::sendResponse() {
  ssize_t n;

  n = response_.sendRawData(sock_fd_);
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
  if (response_.getRawReponse().empty()) {
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
const ServerConfig* Session::findServer() const {
  // get ip and port
  sockaddr_in addr;
  socklen_t addrlen = sizeof(sockaddr_in);
#ifndef UNIT_TEST
  getsockname(sock_fd_, reinterpret_cast<struct sockaddr*>(&addr), &addrlen);
  in_addr_t ip = addr.sin_addr.s_addr;
  uint16_t port = addr.sin_port;
#else
  // just for unit_test
  (void)addr;
  (void)addrlen;
  in_addr_t ip = 0x12345678;
  uint16_t port = 0x1234;
  const_cast<Request&>(request_).headers_["host"] = "localhost:8080";
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
      if ((itr_listen->first == INADDR_ANY || itr_listen->first == ip) &&
          itr_listen->second == port) {
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
        if (itr_host == request_.getHeaders().end() ||
            isServerNameMatch(itr_host->second, *itr_sn)) {
          // itr_host->second == *itr_sn) {
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
void Session::startReadingFromFile() {
  // findfile
  std::string filepath = findFile();
  if (filepath.empty()) {
    createErrorResponse(HTTP_404);
    return;
  }

  file_fd_ = open(filepath.c_str(), O_RDONLY);  // toriaezu
  if (file_fd_ == -1) {
    std::cout << "[error] open failure" << std::endl;
    createErrorResponse(HTTP_403);
    return;
  }
  fcntl(file_fd_, F_SETFL, O_NONBLOCK);

  // [TEMP] create response header
  response_.appendRawData("HTTP/1.1 200 0K\r\n", 17);
  response_.appendRawData("Content-Type: text/html; charset=UTF-8\r\n", 40);
  response_.appendRawData("\r\n", 2);

  status_ = SESSION_FOR_FILE_READ;
}

// find requested file
std::string Session::findFile() const {
  struct stat pathstat;
  std::string rootpath = findRoot();
  std::string filepath = rootpath + request_.getUri();

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
    std::cout << "[error] failed to read from file" << std::endl;
    if (retry_count_ == RETRY_TIME_MAX) {
      retry_count_ = 0;

      // close file and make error responce
      std::cout << "[error] close file" << std::endl;
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

  // check if reached eof
  if (n == 0) {
    close(file_fd_);                    // close
    status_ = SESSION_FOR_CLIENT_SEND;  // set for send response
    return 0;
  }

  // append data to response
  response_.appendRawData(read_buf, n);

  return 0;
}

/*
** directory listing creators (todo!!)
*/

void Session::startDirectoryListing() {
  response_.appendRawData("autoindex!!!!!!!!!!!!!!!!!!", 27);
  status_ = SESSION_FOR_CLIENT_SEND;
}

/*
** cgi handlers (TODO!!)
*/

void Session::startCgiProcess() {
  HTTPStatusCode http_status = cgi_handler_.createCgiProcess();
  if (http_status != HTTP_200) {
    std::cout << "[error] failed to create cgi process" << std::endl;
    createErrorResponse(http_status);
  }
  status_ = SESSION_FOR_CGI_WRITE;
}

// write to cgi process (TODO!!)
int Session::writeToCgi() {
  ssize_t n;

  // write to cgi process
  n = cgi_handler_.writeToCgi(request_.getBuf().c_str(),
                              request_.getBuf().length());

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
  response_.appendRawData(read_buf, n);

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
  n = write(file_fd_, request_.getBuf().c_str(), request_.getBuf().length());

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
