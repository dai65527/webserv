/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/03 20:22:22 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/05/03 23:36:07 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <map>
#include <vector>
#include <string>

#include "HttpStatusCode.hpp"

class Response {
#ifdef UNIT_TEST
 public:
#else
 private:
#endif
  // map (status code) -> message (ex: HTTP_404 -> "Not Found")
  static std::map<HTTPStatusCode, std::string> response_code_message_;
  static void initResponseCodeMessage();

  std::string status_header_;
  std::vector<char> body_;
  int send_progress_; // 0: first send, 1: sending header, 2: sending body
  size_t bytes_already_sent_;
  bool connection_to_close_;

  int createCompleteHeader(bool is_trace = false);

  Response(Response const& other);
  Response& operator=(Response const& other);

 public:
  Response();
  virtual ~Response();

  bool isConnectionToClose() const;
  void setConnectionToClose();
  void resetAll();
  int addHeader(const std::string& key, const std::string& value);
  int createStatusLine(HTTPStatusCode http_status_);
  int createStatusLine(const std::string& value);
  void createErrorStatusLine(HTTPStatusCode http_status);
  int createDefaultErrorResponse(HTTPStatusCode http_status_);
  int appendToBody(const std::string& data);
  int appendToBody(const char* data, size_t len);
  ssize_t sendData(int sock_fd, bool header_only = false, bool is_trace = false);

  //getters
  const std::string& getStatusHeader() const;
  size_t getBytesAlreadySent() const;
};

#endif /* RESPONSE_HPP */
