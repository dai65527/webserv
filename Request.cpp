/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/10 23:36:10 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/24 02:32:55 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

#include <sys/socket.h>

#include <iostream>
#include <stdexcept>

Request::Request() {}

Request::~Request() {}

/* getters */
const std::string& Request::getBuf() const { return buf_; }
const std::string& Request::getMethod() const { return method_; }
const std::string& Request::getUri() const { return uri_; }
const std::map<std::string, std::string>& Request::getHeaders() const {
  return headers_;
}
const std::string& Request::getBody() const { return body_; }

int Request::receive(int sock_fd) {
  int ret;
  char read_buf[BUFFER_SIZE];
  ret = recv(sock_fd, read_buf, BUFFER_SIZE, 0);
  if (ret < 0) return -1;
  buf_.append(read_buf, ret);
  parseRequest();
  return ret;
}

void Request::eraseBuf(ssize_t n) { buf_.erase(0, n); }

void Request::eraseBody(ssize_t n) { body_.erase(0, n); }

size_t Request::parseMethod() {
  size_t pos = 0;
  while (buf_[pos] != ' ' && buf_[pos] != '\r') {
    ++pos;
  }
  method_ = buf_.substr(0, pos);
  return pos;
}

size_t Request::parseUri(size_t pos) {
  while (buf_[pos] == ' ' && buf_[pos] != '\r') {
    ++pos;
  }
  size_t copy_begin = pos;
  while (buf_[pos] != ' ' && buf_[pos] != '\r') {
    ++pos;
  }
  uri_ = buf_.substr(copy_begin, pos - copy_begin);
  return pos;
}

size_t Request::checkRequestLine(size_t pos) {
  std::string methods[8] = {"GET",    "HEAD",    "POST",    "PUT",
                            "DELETE", "CONNECT", "OPTIONS", "TRACE"};
  for (int i = 0; i < 8; ++i) {
    if (method_ == methods[i]) {
      break;
    }
    if (i == 7) {
      std::cout << "405 Mehod Not Allowed" << std::endl;  // Todo make response
      return 1;
    }
  }
  while (buf_[pos] == ' ' && buf_[pos] != '\r') {
    ++pos;
  }
  size_t copy_begin = pos;
  std::string protocol;
  while (buf_[pos] != ' ' && buf_[pos] != '\r') {
    ++pos;
  }
  protocol = buf_.substr(copy_begin, pos - copy_begin);
  if (protocol != "HTTP/1.1") {
    return 1;
  }
  if (buf_[pos] != '\r' && buf_[pos] != '\n') {
    return 1;
  }
  return 0;
}

size_t Request::parseRequestLine() {
  /* parse first line */
  size_t pos;
  if (method_.empty()) {
    pos = parseMethod();
  }
  if (uri_.empty()) {
    pos = parseUri(pos);
  }
  if (checkRequestLine(pos)) {
    throw std::runtime_error("Error in request line"); // TODO: std::exception?
  }
  return pos;
}

int Request::parseRequest() {
  return (parseRequestLine());
}

// int Request::parseRequest() {
//   // parse first line (request line)
//   std::string str;
//   std::string::iterator itr = buf_.begin();
//   std::string::iterator copy_begin;
//   int cnt = 0;
//   while (*itr != '\n') {
//     copy_begin = itr;
//     while (*itr != ' ' || *itr != '\n') itr++;
//     buf_.copy(str, itr - copy_begin, copy_begin);
//     cnt++;
//     switch (cnt) {
//       case 1:
//         /// str in {GET, PUT, ....}
//         // if not throw std::runtime_error("invalid method")
//         method_ = str;
//         break;
//       case 2:
//         uri_ = str;
//         break;
//       case 3:
//         if (str.compare("HTTP/1.1"))
//           // throw std::runtime_error("invalid protoclol version")
//           break;
//       default:
//           // throw std::runtime_error("invalid request line")
//           ;
//     }
//     itr++;
//   }
//   if (itr == buf_.end())
//     return 0;  // HTTP/1.1 requires at least host: ???->error
//   itr++;

//   // get request header field)
//   copy_begin = itr;
//   while (itr != buf_.end()) {
//     if (*itr == '\n') break;
//     itr++;
//   }
//   buf_.copy(headers_, itr - copy_begin,
//             copy_begin) if (itr == buf_.end()) return 0;

//   // get request body
//   copy_begin = itr;
//   while (itr != buf_.end()) {
//     break;
//     itr++;
//   }
//   buf_.copy(body_, itr - copy_begin, copy_begin) return 0;
// }