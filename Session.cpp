/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 23:21:37 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/24 12:37:36 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Session.hpp"

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

Session::Session(int sock_fd)
    : sock_fd_(sock_fd), retry_count_(0), status_(SESSION_FOR_CLIENT_RECV) {}

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
// const LocationConfig& Session::getConfig() const {return config_;}
const SessionStatus& Session::getStatus() const { return status_; }
const Request& Session::getRequest() const { return request_; }
const Response& Session::getResponse() const { return response_; }
const CgiHandler& Session::getCgiHandler() const { return cgi_handler_; }

// void Session::setConfig(const std::list<LocationConfig>& config_list) {}

/*
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

int Session::checkSelectedAndExecute(fd_set* rfds, fd_set* wfds) {
  if (FD_ISSET(sock_fd_, rfds)) {
    if (receiveRequest() == -1) {
      return (-1);
    } else {
      std::cout << "[webserv] received request data" << std::endl;
      return (1);
    }
  } else if (FD_ISSET(file_fd_, rfds)) {
    if (readFromFile() == -1) {
      return (-1);
    } else {
      std::cout << "[webserv] read data from file" << std::endl;
      return (1);
    }
  } else if (FD_ISSET(file_fd_, wfds)) {
    if (writeToFile() == -1) {
      return (-1);
    } else {
      std::cout << "[webserv] write data to file" << std::endl;
      return (1);
    }
  } else if (FD_ISSET(cgi_handler_.getInputFd(), wfds)) {
    if (writeToCgi() == -1) {
      return (-1);
    } else {
      std::cout << "[webserv] wrote data to cgi" << std::endl;
      return (1);
    }
  } else if (FD_ISSET(cgi_handler_.getOutputFd(), rfds)) {
    if (readFromCgi() == -1) {
      return (-1);
    } else {
      std::cout << "[webserv] read data from cgi" << std::endl;
      return (1);
    }
  } else if (FD_ISSET(sock_fd_, wfds)) {
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

void Session::startCreateResponse(HTTPStatusCode http_status) {
  if (http_status != HTTP_200) {
    // error msg
    response_.createErrorResponse(http_status);
    status_ = SESSION_FOR_CLIENT_SEND;
  }

  switch (request_.checkResponseType()) {
    // read from file
    case 0:
      file_fd_ = open("hello.txt", O_RDONLY);  // toriaezu
      if (file_fd_ == -1) {
        std::cout << "[error] open failure" << std::endl;
        response_.createErrorResponse(HTTP_404);
        status_ = SESSION_FOR_CLIENT_SEND;
        break;
      }
      fcntl(file_fd_, F_SETFL, O_NONBLOCK);
      status_ = SESSION_FOR_FILE_READ;
      break;

    // write to file
    case 1:
      file_fd_ = open("./test_req.txt", O_RDWR | O_CREAT, 0644);  // toriaezu
      if (file_fd_ == -1) {
        response_.createErrorResponse(HTTP_503);
        status_ = SESSION_FOR_CLIENT_SEND;
        break;
      }
      fcntl(file_fd_, F_SETFL, O_NONBLOCK);
      status_ = SESSION_FOR_FILE_WRITE;
      break;

    // create cgi process
    case 2:
      http_status = cgi_handler_.createCgiProcess();  //
      if (http_status != HTTP_200) {
        std::cout << "[error] failed to create cgi process" << std::endl;
        response_.createErrorResponse(http_status);
        status_ = SESSION_FOR_CLIENT_SEND;
        break;
      }
      status_ = SESSION_FOR_CGI_WRITE;
      break;

    // oumugaeshi -> error 501
    default:
      // response_.appendRawData(request_.getBuf().c_str(),
      // request_.getBuf().length());
      response_.createErrorResponse(HTTP_501);
      status_ = SESSION_FOR_CLIENT_SEND;
  }
}

// return -1 on failure
int Session::receiveRequest() {
  int ret;

  // receive returns...
  // -2:failed to receive, -1:bad request, 0:received all, 1:continue
  ret = request_.receive(sock_fd_);
  if (ret == -2) {
    if (retry_count_ == RETRY_TIME_MAX) {
      // then try to send return internal server error
      startCreateResponse(HTTP_500);
    } else {
      retry_count_++;
    }
    return 0;
  }
  retry_count_ = 0;
  if (ret == -1) {
    startCreateResponse(HTTP_400);
  } else if (ret == 0) {
    startCreateResponse(HTTP_200);
  }
  return 0;
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
      // response_buf_ =
      // "500 internal server error";  // TODO: make response func

      // to send error response to client
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
    close(file_fd_);                    // close pipefd
    status_ = SESSION_FOR_CLIENT_SEND;  // set for send response
    return 0;
  }

  // append data to response
  response_.appendRawData(read_buf, n);

  return 0;
}

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