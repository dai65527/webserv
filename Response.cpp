/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 23:50:27 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/24 22:45:57 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

#include <sys/socket.h>

Response::Response() {}

Response::Response() { initResponseCodeMessage(); }

Response::~Response() {}

int Response::appendRawData(const char* data, size_t len) {
  raw_response_.append(data, len);
  return 0;
}

const std::string& Response::getRawReponse() const { return raw_response_; }

ssize_t Response::sendRawData(int sock_fd) {
  ssize_t n = send(sock_fd, raw_response_.c_str(), raw_response_.length(), 0);

  // delete sent data
  if (n > 0) {
    raw_response_.erase(0, n);
  }
  return n;
}

void Response::initResponseCodeMessage() {
  static bool flg_set;
  if (!flg_set) {
    flg_set = true;
    response_code_message_[HTTP_100] = "Continue";
    response_code_message_[HTTP_101] = "Switching Protoclols";
    response_code_message_[HTTP_102] = "Processing";   // RFC2518
    response_code_message_[HTTP_103] = "Early Hints";  // RFC8297
    response_code_message_[HTTP_200] = "OK";
    response_code_message_[HTTP_201] = "Created";
    response_code_message_[HTTP_202] = "Accepted";
    response_code_message_[HTTP_203] = "Non-Authoritative Information";
    response_code_message_[HTTP_204] = "No Content";
    response_code_message_[HTTP_205] = "Reset Content";
    response_code_message_[HTTP_206] = "Partial Content";
    response_code_message_[HTTP_207] = "Multi-Status";      // RFC4918
    response_code_message_[HTTP_208] = "Already Reported";  // RFC5842
    response_code_message_[HTTP_226] = "IM Used";           // RFC3229
    response_code_message_[HTTP_300] = "Multiple choice";
    response_code_message_[HTTP_301] = "Moved Parmanently";
    response_code_message_[HTTP_302] = "Found";
    response_code_message_[HTTP_303] = "See Other";
    response_code_message_[HTTP_304] = "Not Modified";
    response_code_message_[HTTP_305] = "Use Proxy";
    response_code_message_[HTTP_306] = "Unused";
    response_code_message_[HTTP_307] = "Temporary Redirect";
    response_code_message_[HTTP_308] = "Permanent Redirect";  // RFC7538
    response_code_message_[HTTP_400] = "Bad Request";
    response_code_message_[HTTP_401] = "Unauthorized";
    response_code_message_[HTTP_402] = "Payment Required";
    response_code_message_[HTTP_403] = "Forbidden";
    response_code_message_[HTTP_404] = "Not Found";
    response_code_message_[HTTP_405] = "Method Not Allowed";
    response_code_message_[HTTP_406] = "Not Acceptable";
    response_code_message_[HTTP_407] = "Proxy Authentication Required";
    response_code_message_[HTTP_408] = "Request Timeout";
    response_code_message_[HTTP_409] = "Conflict";
    response_code_message_[HTTP_410] = "Gone";
    response_code_message_[HTTP_411] = "Length Required";
    response_code_message_[HTTP_412] = "Precondition Failed";
    response_code_message_[HTTP_413] = "Payload Too Large";
    response_code_message_[HTTP_414] = "URI Too Long";
    response_code_message_[HTTP_415] = "Unsupported Media Type";
    response_code_message_[HTTP_416] = "Range Not Satisfiable";
    response_code_message_[HTTP_417] = "Exception Failed";
    response_code_message_[HTTP_418] = "I'm a teapot";
    response_code_message_[HTTP_421] = "Misdirected Request";   // RFC7540
    response_code_message_[HTTP_422] = "Unprocessable Entity";  // RFC4918
    response_code_message_[HTTP_423] = "Locked";                // RFC4918
    response_code_message_[HTTP_424] = "Failed Dependency";     // RFC4918
    response_code_message_[HTTP_425] = "Too Early";             // RFC8470
    response_code_message_[HTTP_426] = "Upgrade Required";
    response_code_message_[HTTP_428] = "Precondition Required";  // RFC6585
    response_code_message_[HTTP_429] = "Too Many Requests";      // RFC6585
    response_code_message_[HTTP_431] =
        "Request Header Fields Too Large";  // RFC6585
    response_code_message_[HTTP_451] =
        "Unabailable For Legal Reasons";  // RFC7225
    response_code_message_[HTTP_500] = "Internal Server Error";
    response_code_message_[HTTP_501] = "Not Implemented";
    response_code_message_[HTTP_502] = "Bad Gateway";
    response_code_message_[HTTP_503] = "Service Unavailable";
    response_code_message_[HTTP_504] = "Gateway Timeout";
    response_code_message_[HTTP_505] = "HTTP Version Not Supported";
    response_code_message_[HTTP_506] = "Variant Also Negotiates";     // RFC2295
    response_code_message_[HTTP_507] = "Insufficient Storage";        // RFC4918
    response_code_message_[HTTP_508] = "HTTP Version Not Supported";  // RFC5842
    response_code_message_[HTTP_510] = "Not Extended";                // RFC2774
    response_code_message_[HTTP_511] =
        "Network Authentication Required";  // RFC6585
  }
}
