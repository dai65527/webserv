/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parseRequest.cc                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 01:10:10 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/24 12:11:45 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "Request.hpp"
#include "gtest.h"

class test_parseRequest : public ::testing::Test {
 protected:
  bool flg_thrown = false;
  Request request;

  // 各ケース共通の前処理を書く
  virtual void SetUp() {}

  // 各ケース共通の後処理を書く
  virtual void TearDown() {}
};

TEST_F(test_parseRequest, reqlineOK) {
  request.buf_ = "GET / HTTP/1.1\r\n";
  EXPECT_NO_THROW(request.parseRequest());
  EXPECT_EQ(request.method_, "GET");
  EXPECT_EQ(request.uri_, "/");
}

TEST_F(test_parseRequest, reqlineOK2) {
  request.buf_ = "HEAD /index.html HTTP/1.1\r\n";
  EXPECT_NO_THROW(request.parseRequest());
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
}

TEST_F(test_parseRequest, protocolFail) {
  request.buf_ = "GET / HTTP/1.2\r\n";
  EXPECT_THROW(request.parseRequest(), std::runtime_error);
}

TEST_F(test_parseRequest, linefeedFail) {
  request.buf_ = "GET / HTTP/1.1\n";
  EXPECT_THROW(request.parseRequest(), std::runtime_error);
}

TEST_F(test_parseRequest, methodFail) {
  request.buf_ = "GUT / HTTP/1.1\r\n";
  EXPECT_THROW(request.parseRequest(), std::runtime_error);
}

TEST_F(test_parseRequest, methodFail2) {
  request.buf_ = " GET / HTTP/1.1\r\n";
  EXPECT_THROW(request.parseRequest(), std::runtime_error);
}

// TEST_F(test_parseRequest, headersOK) {
//   request.buf_ = "HEAD /index.html HTTP/1.1\r\nHost: localhost\r\n\r\n";
//   EXPECT_NO_THROW(request.parseRequest());
//   EXPECT_EQ(request.method_, "HEAD");
//   EXPECT_EQ(request.uri_, "/index.html");
//   EXPECT_EQ(request.headers_, "Host: localhost");
// }