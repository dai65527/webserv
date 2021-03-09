/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sessions.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 23:21:37 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/09 21:12:33 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include "Session.hpp"

/*
** default constructor
*/

Session::Session() : status_(SESSION_NOT_INIT), sock_fd_(0){};

/*
** destructor
** do nothng
*/

Session::~Session(){};

/*
** constructor
**
** initialize fd and status
**    - status is initialized SESSION_FOR_CLIENT_RECV first
*/

Session::Session(int sock_fd)
    : status_(SESSION_FOR_CLIENT_RECV), sock_fd_(sock_fd){};

Session::Session(const Session& ref) { *this = ref; }

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

void Session::setConfig(const std::list<LocationConfig>& config_list) {}

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
    if (request_.appendRa == -1) {
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
    if (cgi_handler_.writeToCgi() == -1) {
      return (-1);
    } else {
      std::cout << "[webserv] wrote data to cgi" << std::endl;
      return (1);
    }
  } else if (FD_ISSET(cgi_handler_.getOutputFd(), rfds)) {
    if (cgi_handler_.readFromCgi() == -1) {
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

void Session::startCreateResponse() {}
void Session::closeConnection() {}
