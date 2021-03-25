/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/10 23:36:10 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/26 00:40:55 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

#include <sys/socket.h>

#include <algorithm>
#include <iostream>

Request::Request() : flg_request_line_(0), flg_header_field_(0), pos_prev_(0) {
  headers_["accept-charsets"] = "";
  headers_["accept-language"] = "";
  headers_["allow"] = "";
  headers_["authorization"] = "";
  headers_["content-language"] = "";
  headers_["content-length"] = "";
  headers_["content-location"] = "";
  headers_["content-type"] = "";
  headers_["date"] = "";
  headers_["host"] = "";
  headers_["last-modified"] = "";
  headers_["location"] = "";
  headers_["referer"] = "";
  headers_["retry-after"] = "";
  headers_["server"] = "";
  headers_["transfer-encoding"] = "";
  headers_["user-agent"] = "";
  headers_["www-authenticate"] = "";
}

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
  ssize_t pos_buf;
  int ret;
  if (!flg_request_line_) {
    if ((pos_buf = getRequestLine()) == -1) {
      return 1;  // 1: continue to receive (will be set to select again)
    }
    ret = parseRequestLine();
    switch (ret) {
      case -1:
        status_code_ = HTTP_400;
        return -1;
      case -2:
        status_code_ = HTTP_505;
        return -2;
      default:
        pos_begin_header_ = ++pos_buf;
        pos_prev_ = pos_begin_header_;
        break;
    }
  }
  if (buf_.c_str()[pos_begin_header_] != '\0' && !flg_header_field_) {
    pos_buf = pos_prev_;
    if ((pos_buf = getHeaderField(pos_buf)) == -1) {
      return 1;  // 1: continue to receive (will be set to select again)
    }
    ret = parseHeaderField(pos_begin_header_);
  }
  return 0;
}

/* get request line from the input (beginning to /r/n) */
ssize_t Request::getRequestLine() {
  size_t pos = pos_prev_;
  while (buf_.c_str()[pos] != '\0' && buf_[pos] != '\r' && buf_[pos] != '\n') {
    ++pos;
  }
  if (buf_.c_str()[pos] == '\0') {
    pos_prev_ = pos;
    return -1;
  }
  if (buf_[pos] == '\r') {
    ++pos;
  }
  if (buf_[pos] == '\n') {
    flg_request_line_ = 1;
    return pos;
  } else {
    pos_prev_ = pos;
    return -1;
  }
}

ssize_t Request::getHeaderField(size_t pos) {
  while (buf_.c_str()[pos] != 0) {
    if (buf_[pos] == '\r') {
      if (buf_.substr(pos, 4) == "\r\n\r\n") {
        flg_header_field_ = 1;
        return pos + 4;
      }
    }
    ++pos;
  }
  pos_prev_ = pos;
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

int Request::parseHeaderField(size_t pos) {
  while (buf_.c_str()[pos] != '\0') {
    //  while (buf_.c_str()[pos] != '\0' &&
    //        (buf_[pos] == '\t' || buf_[pos] == ' ')) {
    //   ++pos;
    // } keyの前に空白を許すかちょっと微妙（RFC的にはダメだが）
    size_t begin = pos;
    while (buf_.c_str()[pos] != '\0' && buf_[pos] != '\r') {
      ++pos;
    }
    std::string key = buf_.substr(begin, buf_.find(":", begin) - begin);
    for (std::string::iterator itr = key.begin(); itr != key.end(); ++itr)
      *itr = std::tolower(*itr);
    begin += key.length() + 1;
    while (buf_.c_str()[begin] != '\0' &&
           (buf_[begin] == '\t' || buf_[begin] == ' ')) {
      ++begin;
    }
    headers_[key] = buf_.substr(begin, pos - begin);
    if (buf_[pos] == '\r') {
      if (buf_.substr(pos, 4) == "\r\n\r\n") {
        break;
      }
      else
        pos += 2;
    }
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
