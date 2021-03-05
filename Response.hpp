/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/03 20:22:22 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/05 11:33:07 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <map>
#include <string>

#include "HttpStatusCode.hpp"

class Response {
 private:
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
  int appendRawData(char* data);
  int createRawData(char* data);
  const char* getRawReponse() const;
  void incrementBytesAlreadySent(size_t bytes);
};

#endif /* RESPONSE_HPP */
