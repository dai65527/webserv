/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parseRequest.cc                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 01:10:10 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/24 20:06:47 by dhasegaw         ###   ########.fr       */
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

TEST_F(test_parseRequest, reqlineOK1) {
  request.buf_ = "GET / HTTP/1.1\r\n";
  EXPECT_EQ(request.parseRequest(), 0);
  EXPECT_EQ(request.method_, "GET");
  EXPECT_EQ(request.uri_, "/");
}

TEST_F(test_parseRequest, reqlineOK2) {
  request.buf_ = "HEAD /index.html HTTP/10.21\r\n";
  EXPECT_EQ(request.parseRequest(), 0);
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
}

TEST_F(test_parseRequest, splitted_reqlineOK) {
  request.buf_ = "HEAD /index.h";
  EXPECT_EQ(request.parseRequest(), 1);
  EXPECT_EQ(request.request_line_, "HEAD /index.h");
  EXPECT_EQ(request.method_, "");
  EXPECT_EQ(request.uri_, "");
  request.buf_ = "tml HTTP/10.21\r\n";
  EXPECT_EQ(request.parseRequest(), 0);
  EXPECT_EQ(request.request_line_, "HEAD /index.html HTTP/10.21");
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
}

TEST_F(test_parseRequest, methodFail1) {
  request.buf_ = " GET / HTTP/1.1\r\n";
  EXPECT_EQ(request.parseRequest(), -1);
}

TEST_F(test_parseRequest, methodFail2) {
  request.buf_ = "GET1 / HTTP/1.1\r\n";
  EXPECT_EQ(request.parseRequest(), -1);
}

TEST_F(test_parseRequest, protocolFail1) {
  request.buf_ = "GET / HTTP1.2\r\n";
  EXPECT_EQ(request.parseRequest(), -1);
}

TEST_F(test_parseRequest, protocolFail2) {
  request.buf_ = "GET / ATTP/1.2\r\n";
  EXPECT_EQ(request.parseRequest(), -1);
}

TEST_F(test_parseRequest, protocolFail3) {
  request.buf_ = "GET / HTTP/12\r\n";
  EXPECT_EQ(request.parseRequest(), -1);
}

TEST_F(test_parseRequest, protocolFail4) {
  request.buf_ = "GET / HTTP/12.\r\n";
  EXPECT_EQ(request.parseRequest(), -1);
}

// TEST_F(test_parseRequest, headersOK) {
//   request.buf_ = "HEAD /index.html HTTP/1.1\r\nHost: localhost\r\n\r\n";
//   EXPECT_NO_THROW(request.parseRequest());
//   EXPECT_EQ(request.method_, "HEAD");
//   EXPECT_EQ(request.uri_, "/index.html");
//   EXPECT_EQ(request.headers_, "Host: localhost");
// }