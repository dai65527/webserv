/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/10 23:36:10 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/24 11:37:06 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

#include <sys/socket.h>

#include <iostream>

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

/*
** receive
**
** receive request using recv syscall
**
** return val:
**  -2: failed to receive (recv syscall failed)
**  -1: bad request (parse failue)
**   0: end of request (go to create response)
**   1: continue to receive (will be set to select again)
*/

int Request::receive(int sock_fd) {
  int ret;
  char read_buf[BUFFER_SIZE];
  ret = recv(sock_fd, read_buf, BUFFER_SIZE, 0);
  if (ret < 0) {
    return -2;  // recv failed
  }
  buf_.append(read_buf, ret);

  /*
  ** parseRequest
  ** returns...
  **  -1: bad request (parse failue)
  **   0: end of request (go to create response)
  **   1: continue to receive (will be set to select again)
  */
  // return parseRequest();

  // temporary
  // return 0: end of request ( ret = 0 )
  // return 1: request not ended (ret = 1)
  return ret == 1 ? 0 : 1;
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

// int Request::parseRequest() {
//   //parse first line (request line)
//   std::string str;
//   std::string::iterator itr = buf_.begin();
//   std::string::iterator copy_begin;
//   int cnt = 0;
//   while (*itr != '\n')
//   {
//     copy_begin = itr;
//     while (*itr != ' ' || *itr != '\r\n')
//       itr++;
//     buf_.copy(str, itr - copy_begin, copy_begin);
//     cnt++;
//     switch (cnt) {
//       case 1:
//        ///str in {GET, PUT, ....}
//        //if not throw std::runtime_error("invalid method")
//        method_ = str;
//        break ;
//       case 2:
//         uri_ = str;
//         break ;
//       case 3:
//         if (str.compare("HTTP/1.1"))
//           //throw std::runtime_error("invalid protoclol version")
//         break ;
//       default:
//         //throw std::runtime_error("invalid request line")
//         ;
//     }
//     itr++;
//   }
//   if (itr == buf_.end())
//     return 0; //HTTP/1.1 requires at least host: ???->error
//   itr++;

//   //get request header field)
//   copy_begin = itr;
//   while (itr != buf_.end()) {
//     if (*itr == '\r\n')
//       break ;
//     itr++;
//   }
//   buf_.copy(headers_, itr - copy_begin, copy_begin)
//   if (itr == buf_.end())
//     return 0;

//   //get request body
//   copy_begin = itr;
//   while (itr != buf_.end()) {
//       break ;
//     itr++;
//   }
//   buf_.copy(body_, itr - copy_begin, copy_begin)
//   return 0;
// }