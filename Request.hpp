/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/03 10:51:41 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/04/23 23:58:22 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <map>
#include <string>
#include <vector>

extern "C" {
#include "libft.h"
}

#define BUFFER_SIZE 1024
#define RETRY_TIME_MAX 10

/* return value of parseRequest*/
#define REQ_FIN_PARSE_HEADER 2   // finished parsing header
#define REQ_CONTINUE_RECV 1      // continue to receive
#define REQ_FIN_RECV 0           // finished receiving
#define REQ_ERR_RECV -1          // fail syscall of recv
#define REQ_CLOSE_CON -5         // socket closed by client
#define REQ_ERR_HTTP_VERSION -2  // HTTP505
#define REQ_ERR_LEN_REQUIRED -3  // HTTP411
#define REQ_ERR_BAD_REQUEST -4   // HTTP400
#define REQ_ERR_TOO_LARGE -6   // HTTP413

/* value of parse_progress_*/
#define REQ_BEFORE_PARSE 0
#define REQ_FIN_REQUEST_LINE 1
#define REQ_FIN_HEADER_FIELD 2
#define REQ_GOT_CHUNK_SIZE 3

class Session;

class Request {
#ifdef UNIT_TEST
 public:
#else
 private:
#endif
  std::vector<char> buf_;
  int parse_progress_;
  int flg_chunked_;
  ssize_t pos_prev_;
  ssize_t pos_begin_header_;
  ssize_t pos_begin_body_;
  std::string header_field_;
  std::string method_;
  std::string uri_;
  std::string query_;
  std::map<std::string, std::string> headers_;
  std::vector<char> body_;
  unsigned long content_length_;
  unsigned long chunk_size_;

  Request(Request const& other);
  Request& operator=(Request const& other);

 public:
  Request();
  virtual ~Request();

  void resetAll();
  const std::string& getMethod() const;
  const std::string& getUri() const;
  const std::map<std::string, std::string>& getHeaders() const;
  const std::string& getQuery() const;
  size_t getContentLength() const;
  const std::vector<char>& getBody() const;
  int getFlgChunked() const;
  int getParseProgress() const;

  int receive(int sock_fd, Session& session);
  int appendRawData(char* raw_data);
  void eraseBody(ssize_t n);
  int parseRequest(Session& session);

#ifdef UNIT_TEST
 public:
#else
 private:
#endif
  ssize_t findRequestLineEnd();
  int parseRequestLine();
  size_t parseMethod();
  size_t parseUri(size_t pos);
  int checkRequestLine(size_t pos);
  int checkResponseType() const;
  ssize_t findHeaderFieldEnd(size_t pos);
  int parseHeaderField(size_t pos);
  int checkHeaderField();
  ssize_t findBodyEndAndStore();
  ssize_t parseChunkedBody(size_t pos);

  std::string bufToString(size_t begin, size_t end);
  int compareBuf(size_t begin, const char* str);
  int checkBodySize(Session& session);
};

#endif /* REQUEST_HPP */
