/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LogFeeder.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/03 23:23:14 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/05/04 21:56:09 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LogFeeder.hpp"

#include <iostream>

LogFeeder::LogFeeder(const Session& session) : session_(session) {}

LogFeeder::~LogFeeder() {}

/*
** $remote_addr    : クライアントのIPアドレス
** $remote_user    : クライアントのユーザ名
** $time_local     : アクセス日時
** $request        : httpの要求URI
** $status         : http responseのステータス
** $body_bytes_sent: 送信バイト数 to client
** $http_referer   : リファラーURL（遷移元URL）
** $http_user_agent: ユーザエージェント情報（ブラウザ名・バージョン等）
** https://www.mk-mode.com/blog/2013/01/16/nginx-access-log/
*/
void LogFeeder::feedLog() const {
  char time[128];
  // format: Sat, 17 Apr 2021 13:45:20 GMT
  getTimeStamp(time, 128, "%a, %d %b %Y %H:%M:%S %Z");

  std::cout << getIpAddress(session_.getIp()) << " - " << getRemoteUser()
            << " [" << time << "]"
            << " \"" << session_.getMethod() << " " << session_.getUri() << " "
            << "HTTP/1.1" << session_.getQuery() << "\""
            << " " << getResponseStatusCode() << " "
            << session_.getBytesAlreadySent() << " "
            << "\"" << getValueFromRequestHeader("referer") << "\" "
            << "\"" << getValueFromRequestHeader("user-agent") << "\""
            << std::endl;
}

std::string LogFeeder::getRemoteUser() const {
  std::string user =
      session_.getUserPass().substr(0, session_.getUserPass().find(':'));
  if (!user.empty()) {
    return user;
  } else {
    return "-";
  }
}

std::string LogFeeder::getResponseStatusCode() const {
  const std::string status_header = session_.getStatusHeader();
  size_t pos = status_header.find_first_of(" ");
  return status_header.substr(pos + 1, 3);  // shold get 3 digit after HTTP/1.1
}

std::string LogFeeder::getValueFromRequestHeader(const std::string& key) const {
  std::map<std::string, std::string>::const_iterator itr =
      session_.getHeaders().find(key);
  if (itr != session_.getHeaders().end()) {
    return (*itr).second;
  } else {
    return "-";
  }
}
