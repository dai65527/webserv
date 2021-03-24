/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/03 20:22:22 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/24 11:05:34 by dnakano          ###   ########.fr       */
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
  std::string raw_response_;
  HTTPStatusCode status_;
  std::string method_;
  std::string uri_;
  std::map<std::string, std::string> headers_;
  std::string body_;
  size_t bytes_already_sent_;

  Response(Response const& other);
  Response& operator=(Response const& other);

 public:
  Response();
  virtual ~Response();
  int appendRawData(const char* data, size_t len);
  int createErrorResponse(HTTPStatusCode http_status);
  const std::string& getRawReponse() const;
  void incrementBytesAlreadySent(size_t bytes);
};

#endif /* RESPONSE_HPP */
