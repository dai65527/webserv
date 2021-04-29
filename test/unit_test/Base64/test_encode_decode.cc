/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_encode_decode.cc                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/04/29 17:39:45 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "Base64.hpp"
#include "gtest.h"

class test_encode_decode : public ::testing::Test {
 protected:
  virtual void SetUp() {}
  virtual void TearDown() {}
};

TEST_F(test_encode_decode, string_1letter) {
  std::string original = "a";
  std::string encoded;
  std::string decoded;
  encoded = Base64::encode(original);
  decoded = Base64::decode(encoded);
  EXPECT_EQ(encoded, "YQ==");
  EXPECT_EQ(decoded, original);
}

TEST_F(test_encode_decode, string_2letters) {
  std::string original = "ab";
  std::string encoded;
  std::string decoded;
  encoded = Base64::encode(original);
  decoded = Base64::decode(encoded);
  EXPECT_EQ(encoded, "YWI=");
  EXPECT_EQ(decoded, original);
}

TEST_F(test_encode_decode, string_3letters) {
  std::string original = "abc";
  std::string encoded;
  std::string decoded;
  encoded = Base64::encode(original);
  decoded = Base64::decode(encoded);
  EXPECT_EQ(encoded, "YWJj");
  EXPECT_EQ(decoded, original);
}

TEST_F(test_encode_decode, string_4letter) {
  std::string original = "abcd";
  std::string encoded;
  std::string decoded;
  encoded = Base64::encode(original);
  decoded = Base64::decode(encoded);
  EXPECT_EQ(encoded, "YWJjZA==");
  EXPECT_EQ(decoded, original);
}

TEST_F(test_encode_decode, string_5letters) {
  std::string original = "abcde";
  std::string encoded;
  std::string decoded;
  encoded = Base64::encode(original);
  decoded = Base64::decode(encoded);
  EXPECT_EQ(encoded, "YWJjZGU=");
  EXPECT_EQ(decoded, original);
}

TEST_F(test_encode_decode, string_6letters) {
  std::string original = "abcdef";
  std::string encoded;
  std::string decoded;
  encoded = Base64::encode(original);
  decoded = Base64::decode(encoded);
  EXPECT_EQ(encoded, "YWJjZGVm");
  EXPECT_EQ(decoded, original);
}

TEST_F(test_encode_decode, string_7letters) {
  std::string original = "abcdefg";
  std::string encoded;
  std::string decoded;
  encoded = Base64::encode(original);
  decoded = Base64::decode(encoded);
  EXPECT_EQ(encoded, "YWJjZGVmZw==");
  EXPECT_EQ(decoded, original);
}

TEST_F(test_encode_decode, string_42siliconvalley) {
  std::string original = "42siliconvalley";
  std::string encoded;
  std::string decoded;
  encoded = Base64::encode(original);
  decoded = Base64::decode(encoded);
  EXPECT_EQ(encoded, "NDJzaWxpY29udmFsbGV5");
  EXPECT_EQ(decoded, original);
}

TEST_F(test_encode_decode, string_basicauth) {
  std::string original = "user:password";
  std::string encoded;
  std::string decoded;
  encoded = Base64::encode(original);
  decoded = Base64::decode(encoded);
  EXPECT_EQ(encoded, "dXNlcjpwYXNzd29yZA==");
  EXPECT_EQ(decoded, original);
}
