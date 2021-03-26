/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parseRequest.cc                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 01:10:10 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/26 17:02:19 by dhasegaw         ###   ########.fr       */
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

TEST_F(test_parseRequest, reqlineOK1butNoHost) {
  request.buf_ = "GET / HTTP/1.1\r\n\r\n";
  EXPECT_EQ(request.parseRequest(), -1);
  EXPECT_EQ(request.method_, "GET");
  EXPECT_EQ(request.uri_, "/");
}

TEST_F(test_parseRequest, reqlineOK2butNoHost) {
  request.buf_ = "HEAD /index.html HTTP/1.1\r\n\r\n";
  EXPECT_EQ(request.parseRequest(), -1);
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
}

TEST_F(test_parseRequest, splitted_reqlineOKbutNoHost) {
  request.buf_ = "HEAD /index.h";
  EXPECT_EQ(request.parseRequest(), 1);
  EXPECT_EQ(request.method_, "");
  EXPECT_EQ(request.uri_, "");
  request.buf_.append("tml HTTP/1.1\r\n\r\n");
  EXPECT_EQ(request.parseRequest(), -1);
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
  EXPECT_EQ(request.parseRequest(), -2);
}

TEST_F(test_parseRequest, protocolFail2) {
  request.buf_ = "GET / ATTP/1.2\r\n";
  EXPECT_EQ(request.parseRequest(), -2);
}

TEST_F(test_parseRequest, protocolFail3) {
  request.buf_ = "GET / HTTP/12\r\n";
  EXPECT_EQ(request.parseRequest(), -2);
}

TEST_F(test_parseRequest, protocolFail4) {
  request.buf_ = "GET / HTTP/12.\r\n";
  EXPECT_EQ(request.parseRequest(), -2);
}

TEST_F(test_parseRequest, headersOK1) {
  request.buf_ = "HEAD /index.html HTTP/1.1\r\nHost: localhost\r\n\r\n";
  EXPECT_EQ(request.parseRequest(), 0);
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.headers_["host"], "localhost");
}

TEST_F(test_parseRequest, headersOK2) {
  request.buf_ =
      "HEAD /index.html HTTP/1.1\r\nHost: "
      "localhost\r\nLocation:Yokohama\r\n\r\n";
  EXPECT_EQ(request.parseRequest(), 0);
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
}

TEST_F(test_parseRequest, splittedheadersOK) {
  request.buf_ = "HEAD /index.html HTTP/1.1\r\nHost: ";
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("localhost\r\nLocation:Yokohama\r\n\r\n");
  EXPECT_EQ(request.parseRequest(), 0);
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
}

TEST_F(test_parseRequest, splittedheadersagain) {
  request.buf_ = "HEAD /index.html HTTP/1.1\r\nHost: ";
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("ama\r\nlanguage: \t\t");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("en-US\r\n\r\n");
  EXPECT_EQ(request.parseRequest(), 0);
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
}

TEST_F(test_parseRequest, splittedheadersagain2) {
  request.buf_ = "HEAD /index.html HTTP/1.1\r\nHost: ";
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("ama\r\nlanguage: \t\t");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("en-US\r\nContent-length: \t\t");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("300\r\n\r\n");
  EXPECT_EQ(request.parseRequest(), 0);
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
  EXPECT_EQ(request.headers_["content-length"], "300");
}

TEST_F(test_parseRequest, headerWithoutColon) {
  request.buf_ = "HEAD /index.html HTTP/1.1\r\nHost ";
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("localhost\r\nLocation:Yokohama\r\n\r\n");
  EXPECT_EQ(request.parseRequest(), -1);//error
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
}

TEST_F(test_parseRequest, headerWithSpaceBeforeHostKey) {
  request.buf_ = "HEAD /index.html HTTP/1.1\r\n Host:";
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("localhost\r\nLocation:Yokohama\r\n\r\n");
  EXPECT_EQ(request.parseRequest(), -1);//error
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.headers_[" host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
}

TEST_F(test_parseRequest, splittedheadersagain3) {
  request.buf_ = "HEAD /index.html HTTP/1.1\r\n Host: ";
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("ama\r\nlanguage: \t\t");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("en-US\r\nContent-length: \t\t");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("300\r\n\r\n");
  EXPECT_EQ(request.parseRequest(), -1);
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.headers_[" host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
  EXPECT_EQ(request.headers_["content-length"], "300");
}

TEST_F(test_parseRequest, queryOK1) {
  request.buf_ = "HEAD /index.html?a=b HTTP/1.1\r\nHost: ";
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("ama\r\nlanguage: \t\t");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("en-US\r\nContent-length: \t\t");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("300\r\n\r\n");
  EXPECT_EQ(request.parseRequest(), 0);
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.query_["a"], "b");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
  EXPECT_EQ(request.headers_["content-length"], "300");
}

TEST_F(test_parseRequest, queryOK2) {
  request.buf_ = "HEAD /index.html?a=b&hoge=fuga HTTP/1.1\r\nHost: ";
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("ama\r\nlanguage: \t\t");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("en-US\r\nContent-length: \t\t");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("300\r\n\r\n");
  EXPECT_EQ(request.parseRequest(), 0);
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.query_["a"], "b");
  EXPECT_EQ(request.query_["hoge"], "fuga");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
  EXPECT_EQ(request.headers_["content-length"], "300");
}

TEST_F(test_parseRequest, queryOK3) {
  request.buf_ = "HEAD /index.html?a=b&hoge= HTTP/1.1\r\nHost: ";
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("ama\r\nlanguage: \t\t");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("en-US\r\nContent-length: \t\t");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("300\r\n\r\n");
  EXPECT_EQ(request.parseRequest(), 0);
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.query_["a"], "b");
  EXPECT_EQ(request.query_["hoge"], "");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
  EXPECT_EQ(request.headers_["content-length"], "300");
}

TEST_F(test_parseRequest, requestErrorNoProtocol) {
  request.buf_ = "HEAD /index.html\r\n\r\n";
  EXPECT_EQ(request.parseRequest(), -1);
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
}

TEST_F(test_parseRequest, requestErrorNoURI) {
  request.buf_ = "HEAD HTTP/1.1\r\n";
  EXPECT_EQ(request.parseRequest(), -1);
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "HTTP/1.1");
}

TEST_F(test_parseRequest, requestNothing) {
  request.buf_ = "\r\n";
  EXPECT_EQ(request.parseRequest(), -1);
}
