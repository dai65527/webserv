/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LogFeeder.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/03 23:17:57 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/05/04 00:27:27 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGFEEDER_HPP
#define LOGFEEDER_HPP

#include <string>

#include "Request.hpp"
#include "Response.hpp"
#include "Session.hpp"
#include "webserv_utils.hpp"

class LogFeeder {
 private:
  const Session& session_;
	const Request& request_;
	const Response& response_;

  LogFeeder();
  LogFeeder(const LogFeeder& other);
  LogFeeder& operator=(const LogFeeder& other);

 public:
  ~LogFeeder();
  LogFeeder(const Session& session, const Request& request,
            const Response& response);
  void feedLog() const;
	std::string getRemoteUser() const;
  std::string getResponseStatusCode() const;
  std::string getValueFromRequestHeader(const std::string& key) const;
};

#endif /* LOGFEEDER_HPP */
