/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/03 10:51:41 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/24 20:22:46 by dhasegaw         ###   ########.fr       */
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
  int flg_request_line_;
  std::string header_field_;
  std::string method_;
  std::string uri_;
  std::map<std::string, std::string> headers_;
  std::string body_;

  Request(Request const& other);
  Request& operator=(Request const& other);

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
  int parseRequest();

#ifdef UNIT_TEST
 public:
#else
 private:
#endif
  ssize_t getRequestLine();
  int parseRequestLine();
  size_t parseMethod();
  size_t parseUri(size_t pos);
  int checkRequestLine(size_t pos);
  int checkResponseType() const;
  int getHeaderField(size_t pos_buf);
  size_t parseHeaderField(size_t pos);
};

#endif /* REQUEST_HPP */
