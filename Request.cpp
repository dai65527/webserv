/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/10 23:36:10 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/23 11:16:44 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include <sys/socket.h>
#include <iostream>

Request::Request() {}

Request::~Request() {}

/* getters */
const std::string& Request::getBuf() const {return buf_;}
const std::string& Request::getMethod() const {return method_;}
const std::string& Request::getUri() const {return uri_;}
const std::map<std::string, std::string>& Request::getHeaders() const {return headers_;}
const std::string& Request::getBody() const {return body_;}

int Request::receive(int sock_fd) {
  int ret;
  char read_buf[BUFFER_SIZE];
  // while (1)
  // {
    std::cout << "sock_fd in request: " << sock_fd << std::endl;
    ret = recv(sock_fd, read_buf, BUFFER_SIZE, 0);
    std::cout << "ret: " << ret << std::endl;
    if (ret < 0)
      return -1;
    // if (ret == 0)
    //   break ;
    buf_.append(read_buf);
  // }
  // parse_request();
  return ret;
}

void  Request::eraseBuf(ssize_t n) {
  buf_.erase(0, n);
}

void  Request::eraseBody(ssize_t n) {
  body_.erase(0, n);
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