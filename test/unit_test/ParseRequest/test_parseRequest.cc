/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parseRequest.cc                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.2tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 01:10:10 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/29 14:30:36 by dhasegaw         ###   ########.fr       */
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
  EXPECT_EQ(request.parseRequest(), -4);
  EXPECT_EQ(request.method_, "GET");
  EXPECT_EQ(request.uri_, "/");
}

TEST_F(test_parseRequest, reqlineOK2butNoHost) {
  request.buf_ = "HEAD /index.html HTTP/1.1\r\n\r\n";
  EXPECT_EQ(request.parseRequest(), -4);
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
}

TEST_F(test_parseRequest, splitted_reqlineOKbutNoHost) {
  request.buf_ = "HEAD /index.h";
  EXPECT_EQ(request.parseRequest(), 1);
  EXPECT_EQ(request.method_, "");
  EXPECT_EQ(request.uri_, "");
  request.buf_.append("tml HTTP/1.1\r\n\r\n");
  EXPECT_EQ(request.parseRequest(), -4);
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
}

TEST_F(test_parseRequest, methodFail1) {
  request.buf_ = " GET / HTTP/1.1\r\n";
  EXPECT_EQ(request.parseRequest(), -4);
}

TEST_F(test_parseRequest, methodFail2) {
  request.buf_ = "GET1 / HTTP/1.1\r\n";
  EXPECT_EQ(request.parseRequest(), -4);
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
  EXPECT_EQ(request.parseRequest(), 2);
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.headers_["host"], "localhost");
}

TEST_F(test_parseRequest, headersOK2) {
  request.buf_ =
      "HEAD /index.html HTTP/1.1\r\nHost: "
      "localhost\r\nLocation:Yokohama\r\n\r\n";
  EXPECT_EQ(request.parseRequest(), 2);
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
}

TEST_F(test_parseRequest, splittedheadersOK) {
  request.buf_ = "HEAD /index.html HTTP/1.1\r\nHost: ";
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("localhost\r\nLocation:Yokohama\r\n\r\n");
  EXPECT_EQ(request.parseRequest(), 2);
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
}

TEST_F(test_parseRequest, splittedheadersOK2) {
  request.buf_ = "HEAD /index.html HTTP/1.1\r\nHost: ";
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("localhost\r\nLocation:Yokohama\r\n");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("\r\n");
  EXPECT_EQ(request.parseRequest(), 2);
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
  EXPECT_EQ(request.parseRequest(), 2);
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
  EXPECT_EQ(request.parseRequest(), 2);
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
  EXPECT_EQ(request.parseRequest(), -4);//error
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
}

TEST_F(test_parseRequest, headerWithSpaceBeforeHostKey) {
  request.buf_ = "HEAD /index.html HTTP/1.1\r\n Host:";
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("localhost\r\nLocation:Yokohama\r\n\r\n");
  EXPECT_EQ(request.parseRequest(), -4);//error
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
  EXPECT_EQ(request.parseRequest(), -4);
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
  EXPECT_EQ(request.parseRequest(), 2);
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
  EXPECT_EQ(request.parseRequest(), 2);
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
  EXPECT_EQ(request.parseRequest(), 2);
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.query_["a"], "b");
  EXPECT_EQ(request.query_["hoge"], "");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
  EXPECT_EQ(request.headers_["content-length"], "300");
}

TEST_F(test_parseRequest, queryOK4) {
  request.buf_ = "HEAD /index.html?a HTTP/1.1\r\nHost: ";
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("ama\r\nlanguage: \t\t");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("en-US\r\nContent-length: \t\t");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("300\r\n\r\n");
  EXPECT_EQ(request.parseRequest(), 2);
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.query_["a"], "");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
  EXPECT_EQ(request.headers_["content-length"], "300");
}

TEST_F(test_parseRequest, requestErrorNoProtocol) {
  request.buf_ = "HEAD /index.html\r\n\r\n";
  EXPECT_EQ(request.parseRequest(), -4);
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
}

TEST_F(test_parseRequest, requestErrorNoURI) {
  request.buf_ = "HEAD HTTP/1.1\r\n";
  EXPECT_EQ(request.parseRequest(), -4);
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "HTTP/1.1");
}

TEST_F(test_parseRequest, requestNothing) {
  request.buf_ = "\r\n";
  EXPECT_EQ(request.parseRequest(), -4);
}

TEST_F(test_parseRequest, postOK) {
  request.buf_ = "POST /index.html?a HTTP/1.1\r\nHost: ";
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("ama\r\nlanguage: \t\t");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("en-US\r\nContent-length: \t\t");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("300\r\n\r\n");
  EXPECT_EQ(request.parseRequest(), 2);
  EXPECT_EQ(request.method_, "POST");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.query_["a"], "");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
  EXPECT_EQ(request.headers_["content-length"], "300");
}

TEST_F(test_parseRequest, postWithoutLength) {
  request.buf_ = "POST /index.html?a HTTP/1.1\r\nHost: ";
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("ama\r\nlanguage: \t\t");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("en-US\r\nContent-type: \t\t");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("awesome\r\n\r\n");
  EXPECT_EQ(request.parseRequest(), -3);
  EXPECT_EQ(request.method_, "POST");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.query_["a"], "");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
  EXPECT_EQ(request.headers_["content-type"], "awesome");
}

TEST_F(test_parseRequest, postwithBodyOK1) {
  request.buf_ = "POST /index.html?a HTTP/1.1\r\nHost: ";
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("ama\r\nlanguage: \t\t");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("en-US\r\nContent-length: \t\t");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("5\r\n\r\n012345678910");
  EXPECT_EQ(request.parseRequest(), 2);
  EXPECT_EQ(request.method_, "POST");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.query_["a"], "");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
  EXPECT_EQ(request.headers_["content-length"], "5");
  EXPECT_EQ(request.parseRequest(), 0);
  EXPECT_EQ(request.buf_, "01234");
  // EXPECT_EQ(request.body_, "01234");
}

TEST_F(test_parseRequest, postwithBodyOK2) {
  request.buf_ = "POST /index.html?a HTTP/1.1\r\nHost: ";
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("ama\r\nlanguage: \t\t");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("en-US\r\nContent-length: \t\t");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("5\r\n\r\n\r\n\r\n\r\n\r\n");
  EXPECT_EQ(request.parseRequest(), 2);
  EXPECT_EQ(request.method_, "POST");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.query_["a"], "");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
  EXPECT_EQ(request.headers_["content-length"], "5");
  EXPECT_EQ(request.parseRequest(), 0);
  EXPECT_EQ(request.buf_, "\r\n\r\n\r");
  // EXPECT_EQ(request.body_, "\r\n\r\n\r");
}

TEST_F(test_parseRequest, contentLengthZeroOK) {
  request.buf_ = "POST /index.html?a HTTP/1.1\r\nHost: ";
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("ama\r\nlanguage: \t\t");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("en-US\r\nContent-length: \t\t");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("0\r\n\r\n\r\n\r\n\r\n\r\n");
  EXPECT_EQ(request.parseRequest(), 2);
  EXPECT_EQ(request.method_, "POST");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.query_["a"], "");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
  EXPECT_EQ(request.headers_["content-length"], "0");
  // EXPECT_EQ(request.body_, "");
}

TEST_F(test_parseRequest, contentLneghtNegative) {
  request.buf_ = "POST /index.html?a HTTP/1.1\r\nHost: ";
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("ama\r\nlanguage: \t\t");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("en-US\r\nContent-length: \t\t");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("-40\r\n\r\n\r\n\r\n\r\n\r\n");
  EXPECT_EQ(request.parseRequest(), -4);
  EXPECT_EQ(request.method_, "POST");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.query_["a"], "");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
  EXPECT_EQ(request.headers_["content-length"], "-40");
}

TEST_F(test_parseRequest, contentLenghtNonDigit) {
  request.buf_ = "POST /index.html?a HTTP/1.1\r\nHost: ";
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("ama\r\nlanguage: \t\t");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("en-US\r\nContent-length: \t\t");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("AAA\r\n\r\n\r\n\r\n\r\n\r\n");
  EXPECT_EQ(request.parseRequest(), -4);
  EXPECT_EQ(request.method_, "POST");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.query_["a"], "");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
  EXPECT_EQ(request.headers_["content-length"], "AAA");
}

TEST_F(test_parseRequest, bodyAgain) {
  request.buf_ = "POST /index.html?a HTTP/1.1\r\nHost: ";
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("ama\r\nlanguage: \t\t");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("en-US\r\nContent-length: \t\t");
  EXPECT_EQ(request.parseRequest(), 1);
  request.buf_.append("16\r\n\r\n012345678");
  EXPECT_EQ(request.parseRequest(), 2);
  request.buf_.append("9abcdefghij");
  EXPECT_EQ(request.method_, "POST");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.query_["a"], "");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
  EXPECT_EQ(request.headers_["content-length"], "16");
  EXPECT_EQ(request.parseRequest(), 0);
  EXPECT_EQ(request.buf_, "0123456789abcdef");
}
