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

// void Response::incrementBytesAlreadySent(size_t bytes) {}