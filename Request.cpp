/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/10 23:36:10 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/29 22:32:05 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

#include <sys/socket.h>

#include <algorithm>
#include <iostream>

Request::Request() : parse_progress_(0), pos_prev_(0), content_length_(0) {}

Request::~Request() {}

/* getters */
const std::string& Request::getBuf() const { return buf_; }
const std::string& Request::getMethod() const { return method_; }
const std::string& Request::getUri() const { return uri_; }
const std::map<std::string, std::string>& Request::getHeaders() const {
  return headers_;
}
const std::map<std::string, std::string>& Request::getQuery() const {
  return query_;
}
size_t Request::getContentLength() const { return content_length_; }

/*
** receive
**
** receive request using recv syscall
**
** return val:
**  -4: 400 bad request (parse failue)
**  -3: 411 Length Required
**  -2: 505 HTTP Version Not Supported
**  -1: failed to receive (recv syscall failed)
**   0: end of request (go to create response)
**   1: continue to receive (will beß set to select again)
*/

#include <unistd.h>

int Request::receive(int sock_fd) {
#ifndef UNIT_TEST
  int ret;
  char read_buf[BUFFER_SIZE];
  ret = recv(sock_fd, read_buf, BUFFER_SIZE, 0);
  if (ret < 0) {
    return -1;
  }
  write(1, read_buf, ret);
  write(1, "\n", 1);
  buf_.append(read_buf, ret);
#else
  (void)sock_fd;
#endif
  return parseRequest();
}

/* parseRequest
** return values: (negatives are ERRPR)
** REQ_FIN_PARSE_HEADER 2 //finished parsing header
** REQ_CONTINUE_RECV 1 //continue to receive
** REQ_FIN_RECV 0 // finished receiving
** REQ_ERR_HTTP_VERSION -2 //HTTP505
** REQ_ERR_LEN_REQUIRED -3 //HTTP411
** REQ_ERR_BAD_REQUEST -4 //HTTP400
*/
int Request::parseRequest() {
  ssize_t pos_buf;
  int ret;
  if (parse_progress_ == 0) {  // 0: parse not started yet
    if ((pos_buf = findRequestLineEnd()) == -1) {
      return REQ_CONTINUE_RECV;
    }
    ret = parseRequestLine();
    if (ret < 0) {
      return ret;
    }
    if (!buf_.compare(pos_buf, 3, "\n\r\n")) { /* in case of NO header field*/
      return REQ_ERR_BAD_REQUEST;
    }
    pos_begin_header_ = ++pos_buf;
    pos_prev_ = pos_begin_header_;
  }
  if (parse_progress_ == 1) {  // 1: finished parse request line then header
    pos_buf = pos_prev_;
    if ((pos_buf = findHeaderFieldEnd(pos_buf)) == -1) {
      return REQ_CONTINUE_RECV;
    }
    ret = parseHeaderField(pos_begin_header_);
    if (ret < 0) {
      return ret;
    }
    if (content_length_ == 0) {
      return REQ_FIN_PARSE_HEADER;
    }
    pos_begin_body_ = pos_buf;
    return REQ_FIN_PARSE_HEADER;
  }
  if (content_length_ > 0 &&
      parse_progress_ == 2) {  // 2: finished parse header then body
    if (findBodyEndAndStore(pos_begin_body_) < 0) {
      return REQ_CONTINUE_RECV;
    }
  }
  return REQ_FIN_RECV;
}

/* get request line from the input (beginning to /r/n) */
ssize_t Request::findRequestLineEnd() {
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
    parse_progress_ = 1;
    return pos;
  } else {
    pos_prev_ = pos;
    return -1;
  }
}

ssize_t Request::findHeaderFieldEnd(size_t pos) {
  while (buf_.c_str()[pos] != 0) {
    if (buf_[pos] == '\r') {
      if (!buf_.compare(pos, 4, "\r\n\r\n")) {
        parse_progress_ = 2;
        return pos + 4;
      }
    }
    ++pos;
  }
  return -1;
}

ssize_t Request::findBodyEndAndStore(size_t pos) {
  while (buf_.c_str()[pos] != 0) {
    if (pos - pos_begin_body_ == content_length_) {
      buf_.erase(0, pos_begin_body_);
      buf_.erase(pos - pos_begin_body_, pos);
      return 0;
    }
    ++pos;
  }
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
  while (buf_.c_str()[pos] == ' ') {
    ++pos;
  }
  size_t copy_begin = pos;
  while (buf_.c_str()[pos] != '\0' && buf_[pos] != ' ' && buf_[pos] != '\r' &&
         buf_[pos] != '?') {
    ++pos;
  }
  uri_ = buf_.substr(copy_begin, pos - copy_begin);
  if (buf_[pos] == '?') { /* parse query */
    while (buf_[pos] != '\0' && buf_[pos] != ' ' && buf_[pos] != '\r') {
      size_t begin = ++pos;
      while (buf_[pos] != '\0' && buf_[pos] != ' ' && buf_[pos] != '\r' &&
             buf_[pos] != '&') {
        ++pos;
      }
      size_t pos_equal = buf_.find("=", begin);
      if (pos_equal == std::string::npos) {
        continue;
      }
      std::string key = buf_.substr(begin, pos_equal - begin);
      ++pos_equal;
      query_[key] = buf_.substr(pos_equal, pos - pos_equal);
      if (buf_[pos] != '&') {
        break;
      }
    }
  }
  return pos;
}

int Request::checkRequestLine(size_t pos) {
  // check method
  if (method_.empty() || uri_.empty()) {
    return -4;
  }
  for (std::string::iterator itr = method_.begin(); itr != method_.end();
       ++itr) {
    if (!std::isupper(*itr)) {
      return -4;
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
  if (protocol.empty()) {
    return -4;
  } else if (protocol != "HTTP/1.1") {
    return -2;
  }
  return 0;
}

int Request::parseHeaderField(size_t pos) {
  while (buf_.c_str()[pos] != '\0') {
    size_t begin = pos;
    while (buf_.c_str()[pos] != '\0' && buf_[pos] != '\r') {
      ++pos;
    }
    size_t pos_colon = buf_.find(":", begin);
    if (pos_colon == std::string::npos) {
      return -4;
    }
    std::string key = buf_.substr(begin, pos_colon - begin);
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
      } else {
        pos += 2;
      }
    }
  }
  return checkHeaderField();
}

int Request::checkHeaderField() {
  if (headers_.find("host") == headers_.end()) {
    return -4;
  }
  std::map<std::string, std::string>::iterator itr_content_length =
      headers_.find("content-length");
  if (method_ == "POST" && itr_content_length == headers_.end()) {
    return -3;
  }
  /*in case of content-length is negative or non digit */
  if (itr_content_length != headers_.end()) {
    for (std::string::iterator itr = itr_content_length->second.begin();
         itr != itr_content_length->second.end(); ++itr) {
      if (!isdigit(*itr)) {
        return -4;
      }
    }
    content_length_ = ft_atoul(itr_content_length->second.c_str());
  }
  return 0;
}

void Request::eraseBuf(ssize_t n) { buf_.erase(0, n); }

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
