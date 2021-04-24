/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_encode_decode.cc                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/04/24 11:33:14 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "UrlEncoder.hpp"
#include "gtest.h"

class test_encode_decode : public ::testing::Test {
 protected:
  virtual void SetUp() {}
  virtual void TearDown() {}
};

TEST_F(test_encode_decode, asciiCharEncoded) {
  std::string encoded;
  std::string decoded;
  encoded = UrlEncoder::encode(':');
  decoded = UrlEncoder::decode(encoded);
  EXPECT_EQ(encoded, "%3A");
  EXPECT_EQ(decoded, ":");
  EXPECT_EQ(decoded.length(), 1);
}

TEST_F(test_encode_decode, asciiCharPercent) {
  std::string encoded;
  std::string decoded;
  encoded = UrlEncoder::encode('%');
  decoded = UrlEncoder::decode(encoded);
  EXPECT_EQ(encoded, "%25");
  EXPECT_EQ(decoded, "%");
  EXPECT_EQ(decoded.length(), 1);
}

TEST_F(test_encode_decode, asciiCharSpace) {
  std::string encoded;
  std::string decoded;
  encoded = UrlEncoder::encode(' ');
  decoded = UrlEncoder::decode(encoded);
  EXPECT_EQ(encoded, "%20");
  EXPECT_EQ(decoded, " ");
  EXPECT_EQ(decoded.length(), 1);
}

TEST_F(test_encode_decode, noSpecialChars) {
  std::string url = "42TokyoAsNo1";
  std::string encoded = UrlEncoder::encode(url);
  std::string decoded = UrlEncoder::decode(encoded);
  EXPECT_EQ(encoded, "42TokyoAsNo1");
  EXPECT_EQ(url, decoded);
}

TEST_F(test_encode_decode, asciiSpecial) {
  std::string url = " :/?#[]@!$&'()*+,;=%";
  std::string encoded = UrlEncoder::encode(url);
  std::string decoded = UrlEncoder::decode(encoded);
  EXPECT_EQ(encoded,
            "%20%3A%2F%3F%23%5B%5D%40%21%24%26%27%28%29%2A%2B%2C%3B%3D%25");
  EXPECT_EQ(url, decoded);
}

TEST_F(test_encode_decode, asciiSpecialCharsMixed) {
  std::string url = ":4/2?Tokyo#[]AS@!$&'()*+,No.;=% 1";
  std::string encoded = UrlEncoder::encode(url);
  std::string decoded = UrlEncoder::decode(encoded);
  EXPECT_EQ(encoded,
            "%3A4%2F2%3FTokyo%23%5B%5DAS%40%21%24%26%27%28%29%2A%2B%2CNo.%3B%"
            "3D%25%201");
  EXPECT_EQ(url, decoded);
}

TEST_F(test_encode_decode, nonAsciiChars) {
  std::string url = "¡™£¢∞§¶•ªº–≠≠¥œ∑´®††¨π“åß∂ƒ©˙∆˚¬…æ≈ç√∫˜µ≤≥≥";
  std::string encoded = UrlEncoder::encode(url);
  std::string decoded = UrlEncoder::decode(encoded);
  EXPECT_EQ(encoded,
            "%C2%A1%E2%84%A2%C2%A3%C2%A2%E2%88%9E%C2%A7%C2%B6%E2%80%A2%C2%AA%"
            "C2%BA%E2%80%93%E2%89%A0%E2%89%A0%C2%A5%C5%93%E2%88%91%C2%B4%C2%AE%"
            "E2%80%A0%E2%80%A0%C2%A8%CF%80%E2%80%9C%C3%A5%C3%9F%E2%88%82%C6%92%"
            "C2%A9%CB%99%E2%88%86%CB%9A%C2%AC%E2%80%A6%C3%A6%E2%89%88%C3%A7%E2%"
            "88%9A%E2%88%AB%CB%9C%C2%B5%E2%89%A4%E2%89%A5%E2%89%A5");
  EXPECT_EQ(url, decoded);
}

TEST_F(test_encode_decode, japanese) {
  std::string url = "私の名前は山田太郎です";
  std::string encoded = UrlEncoder::encode(url);
  std::string decoded = UrlEncoder::decode(encoded);
  EXPECT_EQ(encoded,
            "%E7%A7%81%E3%81%AE%E5%90%8D%E5%89%8D%E3%81%AF%E5%B1%B1%E7%94%B0%"
            "E5%A4%AA%E9%83%8E%E3%81%A7%E3%81%99");
  EXPECT_EQ(url, decoded);
}

TEST_F(test_encode_decode, allMixed) {
  std::string url =
      "∞§ 山田 ¶• ª:,º–≠ 太郎　　≠¥œ42∑´®Tokyo††☺️is¨π“åß( "
      "✌︎'ω')✌︎∂No.ƒ©1";
  std::string encoded = UrlEncoder::encode(url);
  std::string decoded = UrlEncoder::decode(encoded);
  EXPECT_EQ(
      encoded,
      "%E2%88%9E%C2%A7%20%E5%B1%B1%E7%94%B0%20%C2%B6%E2%80%A2%20%C2%AA%3A%2C%"
      "C2%BA%E2%80%93%E2%89%A0%20%E5%A4%AA%E9%83%8E%E3%80%80%E3%80%80%E2%89%A0%"
      "C2%A5%C5%9342%E2%88%91%C2%B4%C2%AETokyo%E2%80%A0%E2%80%A0%E2%98%BA%EF%"
      "B8%8Fis%C2%A8%CF%80%E2%80%9C%C3%A5%C3%9F%28%20%E2%9C%8C%EF%B8%8E%27%CF%"
      "89%27%29%E2%9C%8C%EF%B8%8E%E2%88%82No.%C6%92%C2%A91");
  EXPECT_EQ(url, decoded);
}
