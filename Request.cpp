/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/10 23:36:10 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/15 23:50:16 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include <sys/socket.h>

Request::Request() {}

Request::~Request() {}

/* getters */
const std::string& Request::getBuf() const {return buf_;}
const std::string& Request::getMethod() const {return method_;}
const std::string& Request::getUri() const {return uri_;}
const std::map<std::string, std::string>& Request::getHeaders() const {return headers;}
const std::string& Request::getBody() const {return body_;}

int Request::receive(int sock_fd) {
  int ret;
  char read_buf[BUFFER_SIZE];
  ret = recv(sock_fd, read_buf, BUFFER_SIZE, 0);
  if (ret < 0)
    return -1;
  buf_.append(read_buf);
  parse_request();
  return 0:
}

int Request::appendRawData(char* raw_data) {
  buf_.append(raw_data);
  return (parseRequest())
}

void  Request::eraseBody(ssize_t n) {
  buf_.erase(0, n);
}

int Request::parseRequest() {
  
  return 0;
}