/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UrlEncoder.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 11:12:53 by dnakano           #+#    #+#             */
/*   Updated: 2021/04/24 12:25:27 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UrlEncoder.hpp"

/*
**  Subfunction: asciiCharToNum
**
**  char型の文字コード、 '0', '1', '2', ..., '9', 'A', 'B', 'C', ..., 'F'
**  つまり、           0x30,0x31,0x32, .., 0x39,0x41,0x42,0x42,..., 0x46 を
**  対応する数字として、 0x00,0x01,0x02, ...,0x09,0x0A,0x0B,0x0C,..., 0x0F に
**  変換して返すだけの関数です。
*/

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

/*
**  Encode
**  一文字もらってパーセントエンコーディングして返すだけの関数です。
**  ex).
**  '@' (= 0x40) => "%40"
*/

std::string UrlEncoder::encode(const int c) {
  std::string res = "%";
  const char hexbase[] = "0123456789ABCDEF";
  res.append(hexbase + static_cast<unsigned char>(c) / 0x10, 1);
  res.append(hexbase + static_cast<unsigned char>(c) % 0x10, 1);
  return res;
}

/*
**  Encode
**  受け取った文字列をパーセントエンコードして返す関数です
**  URLに使用できない文字のみエンコードします。
**  ex)
**  "dnakano@student.42tokyo.jp" => "dnakano%40student.42tokyo.jp"
*/

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

/*
**  Decode
**  受け取った文字列のパーセントエンコードをデコードして返す関数です
**  %[0-F][0-F]の値をデコードします
**  ex)
**  "dnakano%40student.42tokyo.jp" => "dnakano@student.42tokyo.jp"
*/

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
