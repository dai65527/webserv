/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/03 10:51:41 by dhasegaw          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2021/03/24 04:29:50 by dhasegaw         ###   ########.fr       */
=======
/*   Updated: 2021/03/24 11:09:31 by dnakano          ###   ########.fr       */
>>>>>>> 4c46a406937d855157ca09c8bbcaace3b854d597
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <map>
#include <string>

#define BUFFER_SIZE 1024
#define RETRY_TIME_MAX 10

class Request {
#ifdef UNIT_TEST
 public:
#else
 private:
#endif
  std::string buf_; /*for temporary saving before parsing*/
  std::string method_;     /* below is the result of parsing*/
  std::string uri_;
  std::map<std::string, std::string> headers_;
  std::string body_;

  Request(Request const& other);
  Request& operator=(Request const& other);

  int parseRequest();

 public:
  Request();
  virtual ~Request();

  const std::string& getBuf() const;
  const std::string& getMethod() const;
  const std::string& getUri() const;
  const std::map<std::string, std::string>& getHeaders() const;
  const std::string& getBody() const;

  int receive(int sock_fd);
  int appendRawData(char* raw_data);
  void eraseBuf(ssize_t n);
  void eraseBody(ssize_t n);

#ifdef UNIT_TEST
 public:
#else
 private:
#endif
  int parseRequest();
  size_t parseRequestLine();
  size_t parseHeaderFields(size_t pos);
  size_t parseMethod();
  size_t parseUri(size_t pos);
  ssize_t checkRequestLine(size_t pos);
  int checkResponseType() const;
};

#endif /* REQUEST_HPP */
