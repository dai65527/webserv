/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 23:50:27 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/24 11:28:36 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response() {}

Response::~Response() {}

int Response::appendRawData(const char* data, size_t len) {
  raw_response_.append(data, len);
  return 0;
}

const std::string& Response::getRawReponse() const { return raw_response_; }

int Response::createErrorResponse(HTTPStatusCode http_status) {
  raw_response_ = "error: " + std::to_string(http_status);
  return 0;
}

// void Response::incrementBytesAlreadySent(size_t bytes) {}