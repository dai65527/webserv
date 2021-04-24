/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 23:50:27 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/04/24 20:13:59 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

#include <sys/socket.h>

#include "webserv_utils.hpp"
#include "webserv_settings.hpp"

std::map<HTTPStatusCode, std::string> Response::response_code_message_;

Response::Response() : send_progress_(0), bytes_already_sent_(0) {
  initResponseCodeMessage();
  connection_to_close_ = false;
}

Response::~Response() {}

void Response::resetAll() {
  send_progress_ = 0;
  bytes_already_sent_ = 0;
  status_header_.clear();
  body_.clear();
}

bool Response::isConnectionToClose() const { return connection_to_close_; }

void Response::setConnectionToClose() { connection_to_close_ = true; }

int Response::createStatusLine(HTTPStatusCode http_status_) {
  // status line
  status_header_ = "HTTP/1.1 ";
  status_header_.append(std::to_string(http_status_));
  status_header_.append(" ");
  status_header_.append(response_code_message_[http_status_]);
  status_header_.append("\r\n");

  // common header
  addHeader("Server", WEBSERV_NAME);

  // time header
  char buf[128];
  // format: Sat, 17 Apr 2021 13:45:20 GMT
  getTimeStamp(buf, 128, "%a, %d %b %Y %H:%M:%S %Z");
  addHeader("Date", buf);

  return 0;
}

int Response::addHeader(const std::string& key, const std::string& value) {
  status_header_.append(key);
  status_header_.append(": ");
  status_header_.append(value);
  status_header_.append("\r\n");
  return 0;
}

void Response::createErrorStatusLine(HTTPStatusCode http_status) {
  // connection to close on these errors
  // https://software.fujitsu.com/jp/manual/manualfiles/m140003/b1ws1026/04z200/b1026-00-03-02-21.html
  switch (http_status) {
    case HTTP_400:
    case HTTP_408:
    case HTTP_411:
    case HTTP_413:
    case HTTP_414:
    case HTTP_500:
    case HTTP_501:
    case HTTP_503:
      connection_to_close_ = true;
    default:
      break;
  }

  // create status line and common header
  createStatusLine(http_status);
}

int Response::createDefaultErrorResponse(HTTPStatusCode http_status) {
  // add Headers
  addHeader("Content-Type", "text/html");

  // create body
  std::string status_msg =
      std::to_string(http_status) + " " + response_code_message_[http_status];
  body_.clear();
  appendToBody("<html>\r\n");
  appendToBody("<head><title>" + status_msg + "</title></ head>\r\n");
  appendToBody("<body bgcolor = \"white\">\r\n");
  appendToBody("<center><h1>" + status_msg + "</h1></center>\r\n");
  appendToBody("<hr><center> nginDX </center>\r\n");
  appendToBody("</body></html>\r\n");

  return 0;
}

int Response::appendToBody(const char* data, size_t len) {
  body_.insert(body_.end(), data, data + len);
  return 0;
}

int Response::appendToBody(const std::string& data) {
  body_.insert(body_.end(), data.begin(), data.end());
  return 0;
}

ssize_t Response::sendData(int sock_fd, bool header_only) {
  ssize_t n;  // response of send syscall
  switch (send_progress_) {
    case 0:  // first time to send
      createCompleteHeader();
      send_progress_ = 1;
      bytes_already_sent_ = 0;
      // no break, to next progress

    case 1:  // sending headers
      n = send(sock_fd, status_header_.c_str() + bytes_already_sent_,
               status_header_.length() - bytes_already_sent_, 0);
      if (n < 0) {
        return -1;
      }

      // count byte already sent
      bytes_already_sent_ += n;
      if (bytes_already_sent_ < status_header_.length()) {
        return 1;  // continue to send
      }

      // sent all header
      status_header_.clear();  // clear for next request
      bytes_already_sent_ = 0;

      // no body to send
      if (body_.empty() || header_only) {
        send_progress_ = 0;  // init for next request
        return 0;            // end
      }

      // to next process (send body)
      send_progress_ = 2;
      return 1;

    case 2:  // sending body
      n = send(sock_fd, &body_[bytes_already_sent_],
               body_.size() - bytes_already_sent_, 0);

      if (n < 0) {
        return -1;
      }

      // count byte already sent
      bytes_already_sent_ += n;
      if (bytes_already_sent_ < body_.size()) {
        return 1;  // continue to send
      }

      // sent all header
      body_.clear();       // clear for next request
      send_progress_ = 0;  // init for next request
      bytes_already_sent_ = 0;
      break;

    default:
      break;
  }
  return 0;
}

// create headers which cannot be defined before create body
int Response::createCompleteHeader() {
  addHeader("Content-Length", std::to_string(body_.size()));
  if (connection_to_close_) {
    addHeader("Connection", "close");
  } else {
    addHeader("Connection", "keep-alive");
  }
  status_header_.append("\r\n");
  return 0;
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
