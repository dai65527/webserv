/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/03 10:51:41 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/03 11:02:10 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <map>
#include <string>

class Request {
 private:
  std::string buf_;
  std::string method_;
  std::string uri_;
  std::map<std::string, std::string> headers_;
  std::string body_;

  Request(Request const& other);
  Request& operator=(Request const& other);

 public:
  Request();
  virtual ~Request();

  std::string getBuf() const;
  std::string getMethod() const;
  std::string getUri() const;
  std::map<std::string, std::string> getHeaders() const;
  std::string getBody() const;
  int appendRawData(char*);

 private:
  int parseRequest();
}

#endif /* REQUEST_HPP */
