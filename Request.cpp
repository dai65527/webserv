/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/10 23:36:10 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/16 11:56:48 by dhasegaw         ###   ########.fr       */
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
  while (1)
  {
    ret = recv(sock_fd, read_buf, BUFFER_SIZE, 0);
    if (ret < 0)
      return -1;
    if (ret == 0)
      break ;
    buf_.append(read_buf);
  }
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
  //parse first line (request line)
  std::string str;
  std::string::iterator itr = _buf.begin();
  int cnt = 0;
  while (*itr != '\n')
  {
     std::string::iterator copy_begin = itr;
    while (*itr != ' ' || *itr != '\n')
      itr++;
    _buf.copy(str, itr - copy_begin, copy_begin);
    cnt++;
    switch (cnt) {
      case 1:
       ///str in {GET, PUT, ....}
       //if not throw std::runtime_error("invalid method")
       method_ = str; 
       break ;
      case 2:
        uri_ = str;
        break ;
      case 3:
        if (str.compare("HTTP/1.1"))
          //throw std::runtime_error("invalid protoclol version")
        break ;
      default:
        //throw std::runtime_error("invalid request line")
    }
    itr++;
  }
  if (itr == buf_.end())
    return 0; //HTTP/1.1 requires at least host: ???
  itr++;

  //get request header field)
  copy_begin = itr;
  while (itr != _buf.end()) {
    if (*itr == '\r\n')
      break ;
    itr++;
  }
  _buf.copy(header_, itr - copy_begin, copy_begin)
  if (itr == buf_.end())
    return 0;
  
  //get request body
  copy_begin = itr;
  while (itr != _buf.end()) {
      break ;
    itr++;
  }
  _buf.copy(body_, itr - copy_begin, copy_begin)
  return 0;
}