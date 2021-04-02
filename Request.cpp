/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/10 23:36:10 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/04/02 13:06:07 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

#include <sys/socket.h>

#include <algorithm>
#include <iostream>

Request::Request()
    : parse_progress_(0),
      flg_chunked_(0),
      pos_prev_(0),
      content_length_(0),
      chunk_size_(0) {}

Request::~Request() {}

/* getters */
const std::vector<char>& Request::getBuf() const { return buf_; }
const std::string& Request::getMethod() const { return method_; }
const std::string& Request::getUri() const { return uri_; }
const std::map<std::string, std::string>& Request::getHeaders() const {
  return headers_;
}
const std::map<std::string, std::string>& Request::getQuery() const {
  return query_;
}
size_t Request::getContentLength() const { return content_length_; }
const std::vector<char>& Request::getBody() const { return body_; }

/* make string from a part of buf*/
std::string Request::bufToString(size_t begin, size_t end) {
  if (end > begin) {
    return std::string(&buf_[begin], &buf_[end]);
  }
  return "";
}

/* compare char literal and a part of buf*/
int Request::compareBuf(size_t begin, const char* str) {
  size_t len = ft_strlen(str);
  for (size_t i = 0; i < len; ++i) {
    if (buf_[i + begin] != str[i]) {
      return 1;
    }
  }
  return 0;
}

/*
** receive
**
** receive request using recv syscall
**
** return values: (negatives are ERRPR)
** REQ_FIN_PARSE_HEADER 2 //finished parsing header
** REQ_CONTINUE_RECV 1 //continue to receive
** REQ_FIN_RECV 0 // finished receiving
** REQ_ERR_HTTP_VERSION -2 //HTTP505
** REQ_ERR_LEN_REQUIRED -3 //HTTP411
** REQ_ERR_BAD_REQUEST -4 //HTTP400
*/

#include <unistd.h>

int Request::receive(int sock_fd) {
#ifndef UNIT_TEST
  int ret;
  char read_buf[BUFFER_SIZE];
  ret = recv(sock_fd, read_buf, BUFFER_SIZE, 0);
  if (ret < 0) {
    return REQ_ERR_RECV;
  }
  write(1, read_buf, ret);
  write(1, "\n", 1);
  buf_.insert(buf_.end(), read_buf, read_buf + ret);
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
    pos_begin_header_ = ++pos_buf;
    pos_prev_ = pos_begin_header_;
  }
  if (parse_progress_ == 1) {  // 1: finished parse request line then header
    // check in case of no header field for first time only
    if (pos_prev_ == pos_begin_header_ && buf_[pos_begin_header_] == '\r' &&
        buf_[pos_begin_header_ + 1] == '\n') {
      return REQ_ERR_BAD_REQUEST;
    }
    pos_buf = pos_prev_;
    if ((pos_buf = findHeaderFieldEnd(pos_buf)) == -1) {
      return REQ_CONTINUE_RECV;
    }
    ret = parseHeaderField(pos_begin_header_);
    if (ret < 0) {
      return ret;
    }
    pos_begin_body_ = pos_buf;
    pos_prev_ = pos_begin_body_;
    return REQ_FIN_PARSE_HEADER;
  }
  /* parse chunked body (Chunked has the priority over Content-length*/
  if (flg_chunked_) {
    pos_buf = pos_prev_;
    return parseChunkedBody(pos_buf);
  }
  /* store body based on content-length value*/
  else if (content_length_ > 0 &&
           parse_progress_ == 2) {  // 2: finished parse header then body
    if (findBodyEndAndStore() < 0) {
      return REQ_CONTINUE_RECV;
    }
  }
  return REQ_FIN_RECV;
}

/* get request line from the input (beginning to /r/n) */
ssize_t Request::findRequestLineEnd() {
  size_t pos = pos_prev_;
  while (pos != buf_.size() && buf_[pos] != '\r' && buf_[pos] != '\n') {
    ++pos;
  }
  if (pos == buf_.size()) {
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
  while (pos != buf_.size()) {
    if (buf_[pos] == '\r') {
      if (!compareBuf(pos, "\r\n\r\n")) {
        parse_progress_ = 2;
        return pos + 4;
      }
    }
    ++pos;
  }
  return -1;
}

ssize_t Request::findBodyEndAndStore() {
  ssize_t excess = buf_.size() - (pos_begin_body_ + content_length_);
  if (excess >= 0) {
    buf_.erase(buf_.end() - excess, buf_.end());
    buf_.erase(buf_.begin(), buf_.begin() + pos_begin_body_);
    body_ = buf_;
    return 0;
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
  while (pos != buf_.size() && buf_[pos] != ' ' && buf_[pos] != '\r') {
    ++pos;
  }
  method_.assign(&buf_[0], &buf_[pos]);
  return pos;
}

size_t Request::parseUri(size_t pos) {
  while (pos != buf_.size() && buf_[pos] == ' ') {
    ++pos;
  }
  size_t copy_begin = pos;
  while (pos != buf_.size() && buf_[pos] != ' ' && buf_[pos] != '\r' &&
         buf_[pos] != '?') {
    ++pos;
  }
  for (size_t i = copy_begin; i < pos; ++i) {
    uri_.push_back(buf_[i]);
  }
  if (buf_[pos] == '?') { /* parse query */
    while (pos != buf_.size() && buf_[pos] != ' ' && buf_[pos] != '\r') {
      size_t begin = ++pos;
      while (pos != buf_.size() && buf_[pos] != ' ' && buf_[pos] != '\r' &&
             buf_[pos] != '&') {
        ++pos;
      }
      size_t pos_equal = begin;
      while (pos_equal != buf_.size()) {
        if (buf_[pos_equal] == '=') {
          break;
        }
        ++pos_equal;
      }
      if (pos_equal == buf_.size()) {
        continue;
      }
      std::string key = bufToString(begin, pos_equal);
      ++pos_equal;
      std::string value = bufToString(pos_equal, pos);
      query_[key] = value;
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
    return REQ_ERR_BAD_REQUEST;
  }
  for (std::string::iterator itr = method_.begin(); itr != method_.end();
       ++itr) {
    if (!std::isupper(*itr)) {
      return REQ_ERR_BAD_REQUEST;
    }
  }

  // check URI
  // 難しいので後回し！

  // check protocol
  while (pos != buf_.size() && buf_[pos] == ' ' && buf_[pos] != '\r') {
    ++pos;
  }
  size_t copy_begin = pos;
  std::string protocol;
  while (pos != buf_.size() && buf_[pos] != ' ' && buf_[pos] != '\r') {
    ++pos;
  }
  for (size_t i = copy_begin; i < pos; ++i) {
    protocol.push_back(buf_[i]);
  }
  if (protocol.empty()) {
    return REQ_ERR_BAD_REQUEST;
  } else if (protocol != "HTTP/1.1") {
    return REQ_ERR_HTTP_VERSION;
  }
  return 0;
}

int Request::parseHeaderField(size_t pos) {
  while (pos != buf_.size()) {
    size_t begin = pos;
    while (pos != buf_.size() && buf_[pos] != '\r') {
      ++pos;
    }
    size_t pos_colon = begin;
    while (pos_colon != buf_.size()) {
      if (buf_[pos_colon] == ':') {
        break;
      }
      ++pos_colon;
    }
    if (pos_colon == buf_.size()) {
      return REQ_ERR_BAD_REQUEST;
    }
    std::string key = bufToString(begin, pos_colon);
    for (std::string::iterator itr = key.begin(); itr != key.end(); ++itr)
      *itr = std::tolower(*itr);
    begin += key.length() + 1;
    while (begin != buf_.size() &&
           (buf_[begin] == '\t' || buf_[begin] == ' ')) {
      ++begin;
    }
    headers_[key] = bufToString(begin, pos);
    if (buf_[pos] == '\r') {
      if (!compareBuf(pos, "\r\n\r\n")) {
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
    return REQ_ERR_BAD_REQUEST;
  }
  std::map<std::string, std::string>::iterator itr_content_length =
      headers_.find("content-length");
  std::map<std::string, std::string>::iterator itr_transfer_encoding =
      headers_.find("transfer-encoding");
  if (itr_transfer_encoding != headers_.end()) {
    for (std::string::iterator itr = itr_transfer_encoding->second.begin();
         itr != itr_transfer_encoding->second.end(); ++itr) {
      *itr = std::tolower(*itr);
    }
    flg_chunked_ = 1;
  }
  /* POST requires content-length or transfer-encoding of chunked*/
  if (method_ == "POST" && itr_content_length == headers_.end() &&
      (itr_transfer_encoding == headers_.end() ||
       itr_transfer_encoding->second != "chunked")) {
    return REQ_ERR_LEN_REQUIRED;
  }
  /*in case of content-length is negative or non digit */
  if (itr_content_length != headers_.end()) {
    for (std::string::iterator itr = itr_content_length->second.begin();
         itr != itr_content_length->second.end(); ++itr) {
      if (!isdigit(*itr)) {
        return REQ_ERR_BAD_REQUEST;
      }
    }
    content_length_ = ft_atoul(itr_content_length->second.c_str());
  }
  return 0;
}

ssize_t Request::parseChunkedBody(size_t pos) {
  size_t begin = pos;
  while (pos != buf_.size()) {
    if (buf_[pos] == '\r' && buf_[pos + 1] == '\n') {
      /* get chunk data size*/
      if (parse_progress_ == 2) {
        std::string chunk_size = bufToString(begin, pos);
        for (std::string::iterator itr = chunk_size.begin();
             itr != chunk_size.end(); ++itr) {
          if (!(isdigit(*itr) || ('a' <= *itr && *itr <= 'f') ||
                ('A' <= *itr && *itr <= 'F'))) {
            return REQ_ERR_BAD_REQUEST;
          }
        }
        chunk_size_ =
            ft_atoul_hexbase(chunk_size.c_str());
        parse_progress_ = 3;
        pos_prev_ = pos + 2;
        return REQ_CONTINUE_RECV;
        /* get chunked data body*/
      } else {
        if (pos - begin > chunk_size_) {
          return REQ_ERR_BAD_REQUEST;
        } else {
          if (chunk_size_ == 0) {  // finish chunked data transfer
            return REQ_FIN_RECV;
          }
          body_.insert(body_.end(), buf_.begin() + begin, buf_.begin() + pos);
          parse_progress_ = 2;
          pos_prev_ = pos + 2;
          return REQ_CONTINUE_RECV;
        }
      }
    }
    ++pos;
  }
  return REQ_CONTINUE_RECV;
}

void Request::eraseBuf(ssize_t n) {
  std::vector<char>::iterator itr = buf_.begin() + n;
  buf_.erase(buf_.begin(), itr);
}

// check
int Request::checkResponseType() const {
  std::string str;
  for (int i = 0; i < 4; ++i) {
    str.push_back(buf_[i]);
  }
  if (!str.compare(0, 4, "read", 0, 4)) {
    return 0;
  } else if (!str.compare(0, 4, "write", 0, 4)) {
    return 1;
  } else if (!str.compare(0, 3, "cgi", 0, 3)) {
    return 2;
  }
  return 42;
}
