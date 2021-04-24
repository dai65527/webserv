/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UrlEncoder.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 11:12:53 by dnakano           #+#    #+#             */
/*   Updated: 2021/04/24 10:59:20 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UrlEncoder.hpp"

static char asciiCharToNum(const char c) {
  if (c >= '0' && c <= '9') {
    return c - '0';
  }
  if (c >= 'A' && c <= 'F') {
    return c - 'A' + 10;
  }
  if (c >= 'a' && c <= 'f') {
    return c - 'a' + 10;
  }
  return 0;
}

std::string UrlEncoder::encode(const int c) {
  std::string res = "%";
  const char hexbase[] = "0123456789ABCDEF";
  res.append(hexbase + static_cast<unsigned char>(c) / 0x10, 1);
  res.append(hexbase + static_cast<unsigned char>(c) % 0x10, 1);
  return res;
}

std::string UrlEncoder::encode(const std::string& str) {
  std::string encoded;
  const char escapedchar[] = ":/?#[]@!$&'()*+,;=% ";

  encoded.reserve(str.length());

  size_t pos = 0;
  size_t pos_encode;
  size_t len = str.length();
  while (pos < len) {
    pos_encode = pos;
    while (pos_encode < len && isprint(str[pos_encode]) &&
           !strchr(escapedchar, str[pos_encode])) {
      pos_encode++;
    }
    encoded.append(str.substr(pos, pos_encode - pos));
    if (pos_encode == len) {
      break;
    }
    encoded.append(encode(str[pos_encode]));
    pos = pos_encode + 1;
  }
  return encoded;
}

std::string UrlEncoder::decode(const std::string& str) {
  std::string decoded;

  decoded.reserve(str.length());

  size_t pos = 0;
  size_t pos_percent = 0;
  size_t len = str.length();
  unsigned char c;
  while (pos < len) {
    pos_percent = str.find('%', pos);
    decoded.append(str.substr(pos, pos_percent - pos));

    // check if string ended
    if (pos_percent == std::string::npos) {
      break;
    }

    // decode
    if (pos_percent + 1 == len) {
      decoded.append("%");
      break;
    }
    if (!isalnum(str[pos_percent + 1])) {
      decoded.append(pos_percent, 2);
      pos = pos_percent + 2;
      continue;
    }
    if (!isalnum(str[pos_percent + 2])) {
      decoded.append(pos_percent, 2);
      pos = pos_percent + 3;
      continue;
    }
    c = asciiCharToNum(str[pos_percent + 1]) * 0x10 +
        asciiCharToNum(str[pos_percent + 2]);
    decoded.append((const char*)&c, 1);
    pos = pos_percent + 3;
  }

  return decoded;
}
