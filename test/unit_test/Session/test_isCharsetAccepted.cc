/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_isCharsetAccepted.cc                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/04/20 18:02:00 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "MainConfig.hpp"
#include "Session.hpp"
#include "gtest.h"

class test_isCharsetAccepted : public ::testing::Test {
 protected:
  MainConfig config;
  Session* session;

  virtual void SetUp() {
    session = new Session(0, config);
    session->request_.headers_["host"] = "localhost:8080";
  }

  // 各ケース共通の後処理を書く
  virtual void TearDown() { delete session; }
};

TEST_F(test_isCharsetAccepted, onlyOneSmall) {
  config.charset_ = "utf-8";
  session->request_.headers_["accept-charset"] = "utf-8";
  EXPECT_TRUE(session->isCharsetAccepted("text/html"));
}

TEST_F(test_isCharsetAccepted, onlyOneBig) {
  config.charset_ = "UTF-8";
  session->request_.headers_["accept-charset"] = "UTF-8";
  EXPECT_TRUE(session->isCharsetAccepted("text/html"));
}

TEST_F(test_isCharsetAccepted, onlyOneSmallBig) {
  config.charset_ = "UTF-8";
  session->request_.headers_["accept-charset"] = "utf-8";
  EXPECT_TRUE(session->isCharsetAccepted("text/html"));
}

TEST_F(test_isCharsetAccepted, onlyOneBigSmall) {
  config.charset_ = "utf-8";
  session->request_.headers_["accept-charset"] = "UTF-8";
  EXPECT_TRUE(session->isCharsetAccepted("text/html"));
}

TEST_F(test_isCharsetAccepted, onlyOneMixed) {
  config.charset_ = "UtF-8";
  session->request_.headers_["accept-charset"] = "uTf-8";
  EXPECT_TRUE(session->isCharsetAccepted("text/html"));
}

TEST_F(test_isCharsetAccepted, onlyOneDiff1) {
  config.charset_ = "utf-8";
  session->request_.headers_["accept-charset"] = "utf-";
  EXPECT_FALSE(session->isCharsetAccepted("text/html"));
}

TEST_F(test_isCharsetAccepted, onlyOneDiff2) {
  config.charset_ = "utf-8";
  session->request_.headers_["accept-charset"] = "utf-82";
  EXPECT_FALSE(session->isCharsetAccepted("text/html"));
}

TEST_F(test_isCharsetAccepted, onlyOneDiff3) {
  config.charset_ = "utf-8";
  session->request_.headers_["accept-charset"] = "utf-9";
  EXPECT_FALSE(session->isCharsetAccepted("text/html"));
}

TEST_F(test_isCharsetAccepted, onlyOneDiff4) {
  config.charset_ = "utf-";
  session->request_.headers_["accept-charset"] = "utf-8";
  EXPECT_FALSE(session->isCharsetAccepted("text/html"));
}

TEST_F(test_isCharsetAccepted, onlyOneDiff5) {
  config.charset_ = "utf-9";
  session->request_.headers_["accept-charset"] = "utf-8";
  EXPECT_FALSE(session->isCharsetAccepted("text/html"));
}

TEST_F(test_isCharsetAccepted, onlyOneDiff6) {
  config.charset_ = "utf-82";
  session->request_.headers_["accept-charset"] = "utf-8";
  EXPECT_FALSE(session->isCharsetAccepted("text/html"));
}

TEST_F(test_isCharsetAccepted, onlyOneWild) {
  config.charset_ = "utf-8";
  session->request_.headers_["accept-charset"] = "*";
  EXPECT_TRUE(session->isCharsetAccepted("text/html"));
}

TEST_F(test_isCharsetAccepted, multiTwoFirst) {
  config.charset_ = "utf-8";
  session->request_.headers_["accept-charset"] = "utf-8, iso-8859-1;q=0.5";
  EXPECT_TRUE(session->isCharsetAccepted("text/html"));
}

TEST_F(test_isCharsetAccepted, multiTwoSecond) {
  config.charset_ = "iso-8859-1";
  session->request_.headers_["accept-charset"] = "utf-8, iso-8859-1;q=0.5";
  EXPECT_TRUE(session->isCharsetAccepted("text/html"));
}

TEST_F(test_isCharsetAccepted, multiTwoWildCardFirst) {
  config.charset_ = "utf-8";
  session->request_.headers_["accept-charset"] =
      "utf-8, iso-8859-1;q=0.5, *;q=0.1";
  EXPECT_TRUE(session->isCharsetAccepted("text/html"));
}

TEST_F(test_isCharsetAccepted, multiTwoWildCardSecond) {
  config.charset_ = "iso-8859-1";
  session->request_.headers_["accept-charset"] =
      "utf-8, iso-8859-1;q=0.5, *;q=0.1";
  EXPECT_TRUE(session->isCharsetAccepted("text/html"));
}

TEST_F(test_isCharsetAccepted, multiTwoWildCardWild) {
  config.charset_ = "shift-jis";
  session->request_.headers_["accept-charset"] =
      "utf-8, iso-8859-1;q=0.5, *;q=0.1";
  EXPECT_TRUE(session->isCharsetAccepted("text/html"));
}

TEST_F(test_isCharsetAccepted, multiTwoWildCardShouldNotMatch1) {
  config.charset_ = "q=0.5";
  session->request_.headers_["accept-charset"] = "utf-8, iso-8859-1;q=0.5";
  EXPECT_FALSE(session->isCharsetAccepted("text/html"));
}

TEST_F(test_isCharsetAccepted, multiTwoWildCardShouldNotMatch1ButNotText) {
  config.charset_ = "q=0.5";
  session->request_.headers_["accept-charset"] = "utf-8, iso-8859-1;q=0.5";
  EXPECT_TRUE(session->isCharsetAccepted("application/json"));
}
