/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/03 20:22:22 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/24 22:46:19 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <map>
#include <string>

#include "HttpStatusCode.hpp"

class Response {
#ifdef UNIT_TEST
 public:
#else
 private:
#endif
  std::string status_header_;
  std::string body_;
  size_t bytes_already_sent_;

  std::string raw_response_;
  HTTPStatusCode status_;
  std::string method_;
  std::string uri_;
  std::map<std::string, std::string> headers_;

  // map (status code) -> message (ex: HTTP_404 -> "Not Found")
  static std::map<HTTPStatusCode, std::string> response_code_message_;

  void initResponseCodeMessage();

  Response(Response const& other);
  Response& operator=(Response const& other);

 public:
  Response();
  virtual ~Response();
  int addHeader(const std::string& key, const std::string& value);
  int createStatusLine(HTTPStatusCode http_status_);
  int appendRawData(const char* data, size_t len);
  ssize_t sendRawData(int sock_fd);
  const std::string& getRawReponse() const;
};

#endif /* RESPONSE_HPP */
