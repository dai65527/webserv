/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/10 23:36:10 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/24 21:24:51 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

#include <sys/socket.h>

#include <iostream>

Request::Request() : flg_request_line_(0) {}

Request::~Request() {}

/* getters */
const std::string& Request::getBuf() const { return buf_; }
const std::string& Request::getMethod() const { return method_; }
const std::string& Request::getUri() const { return uri_; }
const std::map<std::string, std::string>& Request::getHeaders() const {
  return headers_;
}
const std::string& Request::getBody() const { return body_; }
HTTPStatusCode Request::getStatusCode() const { return status_code_; };

/*
** receive
**
** receive request using recv syscall
**
** return val:
**  -3: failed to receive (recv syscall failed)
**  -2: 505 HTTP Version Not Supported
**  -1: 400 bad request (parse failue)
**   0: end of request (go to create response)
**   1: continue to receive (will beß set to select again)
*/

int Request::receive(int sock_fd) {
  int ret;
  char read_buf[BUFFER_SIZE];
  ret = recv(sock_fd, read_buf, BUFFER_SIZE, 0);
  if (ret < 0) {
    return -3;
  }
  buf_.append(read_buf, ret);
  return parseRequest();
}

/* parseRequest
** return values:
**  -2: 505 HTTP Version Not Supported
**  -1: 400 bad request (parse failue)
**   0: end of request (go to create response)
**   1: continue to receive (will be set to select again)
*/
int Request::parseRequest() {
  ssize_t pos_buf = 0;
  if (!flg_request_line_ && ((pos_buf = getRequestLine()) == -1)) {
    return 1;  // 1: continue to receive (will be set to select again)
  }
  int ret;
  // getHeaderField(pos_buf);
  ret = parseRequestLine();
  if (ret == -1)
    status_code_ = HTTP_400;
  else if (ret == -2)
    status_code_ = HTTP_505;
  return ret;
}

/* get request line from the input (beginning to /r/n) */
ssize_t Request::getRequestLine() {
  size_t pos = 0;
  while (buf_.c_str()[pos] != '\0' && buf_[pos] != '\r' && buf_[pos] != '\n') {
    ++pos;
  }
  if (buf_.c_str()[pos] == '\0') {
    return -1;
  }
  if (buf_[pos] == '\r') {
    ++pos;
  }
  if (buf_[pos] == '\n') {
    flg_request_line_ = 1;
    return pos;
  } else
    return -1;
}

int Request::parseRequestLine() {
  if (!method_.empty()) {
    return 0;
  }
  ssize_t pos = 0;
  pos = parseMethod();
  pos = parseUri(pos);
  return checkRequestLine(pos);
}

size_t Request::parseMethod() {
  size_t pos = 0;
  while (buf_.c_str()[pos] != '\0' && buf_[pos] != ' ' && buf_[pos] != '\r') {
    ++pos;
  }
  method_ = buf_.substr(0, pos);
  return pos;
}

size_t Request::parseUri(size_t pos) {
  while (buf_.c_str()[pos] != '\0' && buf_[pos] == ' ' && buf_[pos] != '\r') {
    ++pos;
  }
  size_t copy_begin = pos;
  while (buf_.c_str()[pos] != '\0' && buf_[pos] != ' ' && buf_[pos] != '\r') {
    ++pos;
  }
  uri_ = buf_.substr(copy_begin, pos - copy_begin);
  return pos;
}

int Request::checkRequestLine(size_t pos) {
  // check method
  if (method_.empty()) {
    return -1;
  }
  for (std::string::iterator itr = method_.begin(); itr != method_.end();
       ++itr) {
    if (!std::isupper(*itr)) {
      return -1;
    }
  }

  // check URI
  // 難しいので後回し！

  // check protocol
  while (buf_.c_str()[pos] != '\0' && buf_[pos] == ' ' && buf_[pos] != '\r') {
    ++pos;
  }
  size_t copy_begin = pos;
  std::string protocol;
  while (buf_.c_str()[pos] != '\0' && buf_[pos] != ' ' && buf_[pos] != '\r') {
    ++pos;
  }
  protocol = buf_.substr(copy_begin, pos - copy_begin);
  if (protocol != "HTTP/1.1") {
    return -2;
  }
  return 0;
}

void Request::eraseBuf(ssize_t n) { buf_.erase(0, n); }

void Request::eraseBody(ssize_t n) { body_.erase(0, n); }

// check
int Request::checkResponseType() const {
  if (!buf_.compare(0, 4, "read", 0, 4)) {
    return 0;
  } else if (!buf_.compare(0, 4, "write", 0, 4)) {
    return 1;
  } else if (!buf_.compare(0, 3, "cgi", 0, 3)) {
    return 2;
  }
  return 42;
}

// size_t Request::parseHeaderFields(size_t pos) {
//   if (buf_[pos])
//     ++pos;
//   else
//     throw std::runtime_error("Error: No headers");
//   size_t copy_begin = pos;
//   while (buf_[pos]) {
//     if ((buf_[pos] == '\r') && (buf_[pos + 1] == '\n') &&
//         (buf_[pos + 2] == '\r') && (buf_[pos + 3] == '\n'))
//       break;
//     ++pos;
//   }
//   headers_ = buf_.substr(copy_begin, pos - copy_begin);
//   pos += 4;
//   return pos;
// }

// size_t Request::

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
//        ///str in {GET, PUT, ....}
//        //if not throw std::runtime_error("invalid method")
//        method_ = str;
//        break ;
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
//   buf_.copy(headers_, itr - copy_begin, copy_begin)
//   if (itr == buf_.end())
//     return 0;

//   //get request body
//   copy_begin = itr;
//   while (itr != buf_.end()) {
//     break;
//     itr++;
//   }
//   buf_.copy(body_, itr - copy_begin, copy_begin) return 0;
// }