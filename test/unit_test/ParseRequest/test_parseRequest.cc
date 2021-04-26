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

#include "MainConfig.hpp"
#include "Request.hpp"
#include "Session.hpp"
#include "gtest.h"
#include "webserv_settings.hpp"

class test_parseRequest : public ::testing::Test {
 protected:
  bool flg_thrown;
  Request request;
  MainConfig config;
  Session *session;

  void appendVec(std::vector<char> &vec, const std::string &str) {
    vec.insert(vec.end(), str.begin(), str.end());
  }
  // 各ケース共通の前処理を書く
  virtual void SetUp() {
    flg_thrown = false;
    session = new Session(0, config, false);
  }

  // 各ケース共通の後処理を書く
  virtual void TearDown() {}
};

TEST_F(test_parseRequest, reqlineOK1butNoHost) {
  // appendVec(request.buf_,  "GET / HTTP/1.1\r\n\r\n");
  appendVec(request.buf_, "GET / HTTP/1.1\r\n\r\n");
  EXPECT_EQ(request.parseRequest(*session), -4);
  EXPECT_EQ(request.method_, "GET");
  EXPECT_EQ(request.uri_, "/");
}

TEST_F(test_parseRequest, reqlineOK2butNoHost) {
  // appendVec(request.buf_,  "HEAD /index.html HTTP/1.1\r\n\r\n");
  appendVec(request.buf_, "HEAD /index.html HTTP/1.1\r\n\r\n");
  EXPECT_EQ(request.parseRequest(*session), -4);
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
}

TEST_F(test_parseRequest, splitted_reqlineOKbutNoHost) {
  // appendVec(request.buf_,  "HEAD /index.h");
  appendVec(request.buf_, "HEAD /index.h");
  EXPECT_EQ(request.parseRequest(*session), 1);
  EXPECT_EQ(request.method_, "");
  EXPECT_EQ(request.uri_, "");
  // appendVec(request.buf_, "tml HTTP/1.1\r\n\r\n");
  appendVec(request.buf_, "tml HTTP/1.1\r\n\r\n");
  EXPECT_EQ(request.parseRequest(*session), -4);
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
}

TEST_F(test_parseRequest, methodFail1) {
  appendVec(request.buf_, " GET / HTTP/1.1\r\n");
  EXPECT_EQ(request.parseRequest(*session), -4);
}

TEST_F(test_parseRequest, methodFail2) {
  appendVec(request.buf_, "GET1 / HTTP/1.1\r\n");
  EXPECT_EQ(request.parseRequest(*session), -4);
}

TEST_F(test_parseRequest, protocolFail1) {
  appendVec(request.buf_, "GET / HTTP1.2\r\n");
  EXPECT_EQ(request.parseRequest(*session), -2);
}

TEST_F(test_parseRequest, protocolFail2) {
  appendVec(request.buf_, "GET / ATTP/1.2\r\n");
  EXPECT_EQ(request.parseRequest(*session), -2);
}

TEST_F(test_parseRequest, protocolFail3) {
  appendVec(request.buf_, "GET / HTTP/12\r\n");
  EXPECT_EQ(request.parseRequest(*session), -2);
}

TEST_F(test_parseRequest, protocolFail4) {
  appendVec(request.buf_, "GET / HTTP/12.\r\n");
  EXPECT_EQ(request.parseRequest(*session), -2);
}

TEST_F(test_parseRequest, headersOK1) {
  appendVec(request.buf_,
            "HEAD /index.html HTTP/1.1\r\nHost: localhost\r\n\r\n");
  EXPECT_EQ(request.parseRequest(*session), REQ_FIN_RECV);
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.headers_["host"], "localhost");
}

TEST_F(test_parseRequest, headersOK2) {
  appendVec(request.buf_,
            "HEAD /index.html HTTP/1.1\r\nHost: "
            "localhost\r\nLocation:Yokohama\r\n\r\n");
  EXPECT_EQ(request.parseRequest(*session), REQ_FIN_RECV);
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
}

TEST_F(test_parseRequest, splittedheadersOK) {
  appendVec(request.buf_, "HEAD /index.html HTTP/1.1\r\nHost: ");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "localhost\r\nLocation:Yokohama\r\n\r\n");
  EXPECT_EQ(request.parseRequest(*session), REQ_FIN_RECV);
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
}

TEST_F(test_parseRequest, splittedheadersOK2) {
  appendVec(request.buf_, "HEAD /index.html HTTP/1.1\r\nHost: ");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "localhost\r\nLocation:Yokohama\r\n");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "\r\n");
  EXPECT_EQ(request.parseRequest(*session), REQ_FIN_RECV);
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
}

TEST_F(test_parseRequest, splittedheadersagain) {
  appendVec(request.buf_, "HEAD /index.html HTTP/1.1\r\nHost: ");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "ama\r\nlanguage: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "en-US\r\n\r\n");
  EXPECT_EQ(request.parseRequest(*session), REQ_FIN_RECV);
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
}

TEST_F(test_parseRequest, splittedheadersagain2) {
  appendVec(request.buf_, "HEAD /index.html HTTP/1.1\r\nHost: ");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "ama\r\nlanguage: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "en-US\r\nContent-length: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "300\r\n\r\n");
  EXPECT_EQ(request.parseRequest(*session), 1);
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
  EXPECT_EQ(request.headers_["content-length"], "300");
}

TEST_F(test_parseRequest, headerWithoutColon) {
  appendVec(request.buf_, "HEAD /index.html HTTP/1.1\r\nHost ");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "localhost\r\nLocation:Yokohama\r\n\r\n");
  EXPECT_EQ(request.parseRequest(*session), -4);  // error
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
}

TEST_F(test_parseRequest, headerWithSpaceBeforeHostKey) {
  appendVec(request.buf_, "HEAD /index.html HTTP/1.1\r\n Host:");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "localhost\r\nLocation:Yokohama\r\n\r\n");
  EXPECT_EQ(request.parseRequest(*session), -4);  // error
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.headers_[" host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
}

TEST_F(test_parseRequest, splittedheadersagain3) {
  appendVec(request.buf_, "HEAD /index.html HTTP/1.1\r\n Host: ");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "ama\r\nlanguage: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "en-US\r\nContent-length: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "300\r\n\r\n");
  EXPECT_EQ(request.parseRequest(*session), -4);
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.headers_[" host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
  EXPECT_EQ(request.headers_["content-length"], "300");
}

TEST_F(test_parseRequest, queryOK1) {
  appendVec(request.buf_, "HEAD /index.html?a=b HTTP/1.1\r\nHost: ");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "ama\r\nlanguage: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "en-US\r\nContent-length: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "300\r\n\r\n");
  EXPECT_EQ(request.parseRequest(*session), REQ_CONTINUE_RECV);
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.query_, "a=b");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
  EXPECT_EQ(request.headers_["content-length"], "300");
}

TEST_F(test_parseRequest, queryOK2) {
  appendVec(request.buf_, "HEAD /index.html?a=b&hoge=fuga HTTP/1.1\r\nHost: ");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "ama\r\nlanguage: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "en-US\r\nContent-length: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "300\r\n\r\n");
  EXPECT_EQ(request.parseRequest(*session), REQ_CONTINUE_RECV);
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.query_, "a=b&hoge=fuga");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
  EXPECT_EQ(request.headers_["content-length"], "300");
}

TEST_F(test_parseRequest, queryOK3) {
  appendVec(request.buf_, "HEAD /index.html?a=b&hoge= HTTP/1.1\r\nHost: ");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "ama\r\nlanguage: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "en-US\r\nContent-length: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "300\r\n\r\n");
  EXPECT_EQ(request.parseRequest(*session), REQ_CONTINUE_RECV);
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.query_, "a=b&hoge=");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
  EXPECT_EQ(request.headers_["content-length"], "300");
}

TEST_F(test_parseRequest, queryOK4) {
  appendVec(request.buf_, "HEAD /index.html?a HTTP/1.1\r\nHost: ");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "ama\r\nlanguage: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "en-US\r\nContent-length: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "300\r\n\r\n");
  EXPECT_EQ(request.parseRequest(*session), REQ_CONTINUE_RECV);
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.query_, "a");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
  EXPECT_EQ(request.headers_["content-length"], "300");
}

TEST_F(test_parseRequest, requestErrorNoProtocol) {
  appendVec(request.buf_, "HEAD /index.html\r\n\r\n");
  EXPECT_EQ(request.parseRequest(*session), -4);
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "/index.html");
}

TEST_F(test_parseRequest, requestErrorNoURI) {
  appendVec(request.buf_, "HEAD HTTP/1.1\r\n");
  EXPECT_EQ(request.parseRequest(*session), -4);
  EXPECT_EQ(request.method_, "HEAD");
  EXPECT_EQ(request.uri_, "HTTP/1.1");
}

TEST_F(test_parseRequest, requestNothing) {
  appendVec(request.buf_, "\r\n");
  EXPECT_EQ(request.parseRequest(*session), -4);
}

TEST_F(test_parseRequest, postOK) {
  appendVec(request.buf_, "POST /index.html?a HTTP/1.1\r\nHost: ");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "ama\r\nlanguage: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "en-US\r\nContent-length: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "300\r\n\r\n");
  EXPECT_EQ(request.parseRequest(*session), REQ_CONTINUE_RECV);
  EXPECT_EQ(request.method_, "POST");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.query_, "a");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
  EXPECT_EQ(request.headers_["content-length"], "300");
}

TEST_F(test_parseRequest, postWithoutLength) {
  appendVec(request.buf_, "POST /index.html?a HTTP/1.1\r\nHost: ");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "ama\r\nlanguage: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "en-US\r\nContent-type: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "awesome\r\n\r\n");
  EXPECT_EQ(request.parseRequest(*session), -3);
  EXPECT_EQ(request.method_, "POST");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.query_, "a");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
  EXPECT_EQ(request.headers_["content-type"], "awesome");
}

TEST_F(test_parseRequest, postwithBodyOK1) {
  appendVec(request.buf_, "POST /index.html?a HTTP/1.1\r\nHost: ");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "ama\r\nlanguage: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "en-US\r\nContent-length: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "5\r\n\r\n012345678910");
  EXPECT_EQ(request.parseRequest(*session), REQ_FIN_RECV);
  EXPECT_EQ(request.method_, "POST");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.query_, "a");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
  EXPECT_EQ(request.headers_["content-length"], "5");
  std::string str(request.body_.begin(), request.body_.end());
  EXPECT_EQ(str, "01234");
}

TEST_F(test_parseRequest, postwithBodyOK2) {
  appendVec(request.buf_, "POST /index.html?a HTTP/1.1\r\nHost: ");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "ama\r\nlanguage: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "en-US\r\nContent-length: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "5\r\n\r\n\r\n\r\n\r\n\r\n");
  EXPECT_EQ(request.parseRequest(*session), REQ_FIN_RECV);
  EXPECT_EQ(request.method_, "POST");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.query_, "a");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
  EXPECT_EQ(request.headers_["content-length"], "5");
  std::string str(request.body_.begin(), request.body_.end());
  EXPECT_EQ(str, "\r\n\r\n\r");
}

TEST_F(test_parseRequest, contentLengthZeroOK) {
  appendVec(request.buf_, "POST /index.html?a HTTP/1.1\r\nHost: ");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "ama\r\nlanguage: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "en-US\r\nContent-length: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "0\r\n\r\n\r\n\r\n\r\n\r\n");
  EXPECT_EQ(request.parseRequest(*session), REQ_FIN_RECV);
  EXPECT_EQ(request.method_, "POST");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.query_, "a");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
  EXPECT_EQ(request.headers_["content-length"], "0");
  // EXPECT_EQ(request.body_, "");
}

TEST_F(test_parseRequest, contentLneghtNegative) {
  appendVec(request.buf_, "POST /index.html?a HTTP/1.1\r\nHost: ");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "ama\r\nlanguage: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "en-US\r\nContent-length: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "-40\r\n\r\n\r\n\r\n\r\n\r\n");
  EXPECT_EQ(request.parseRequest(*session), -4);
  EXPECT_EQ(request.method_, "POST");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.query_, "a");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
  EXPECT_EQ(request.headers_["content-length"], "-40");
}

TEST_F(test_parseRequest, contentLenghtNonDigit) {
  appendVec(request.buf_, "POST /index.html?a HTTP/1.1\r\nHost: ");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "ama\r\nlanguage: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "en-US\r\nContent-length: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "AAA\r\n\r\n\r\n\r\n\r\n\r\n");
  EXPECT_EQ(request.parseRequest(*session), -4);
  EXPECT_EQ(request.method_, "POST");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.query_, "a");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
  EXPECT_EQ(request.headers_["content-length"], "AAA");
}

TEST_F(test_parseRequest, bodyAgain) {
  appendVec(request.buf_, "POST /index.html?a HTTP/1.1\r\nHost: ");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "ama\r\nlanguage: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "en-US\r\nContent-length: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "16\r\n\r\n012345678");
  EXPECT_EQ(request.parseRequest(*session), REQ_CONTINUE_RECV);
  appendVec(request.buf_, "9abcdefghij");
  EXPECT_EQ(request.method_, "POST");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.query_, "a");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
  EXPECT_EQ(request.headers_["content-length"], "16");
  EXPECT_EQ(request.parseRequest(*session), 0);
  // std::string str(request.buf_.begin(), request.buf_.end());
  std::string str(request.body_.begin(), request.body_.end());
  EXPECT_EQ(str, "0123456789abcdef");
}

TEST_F(test_parseRequest, transferEncodingOK1) {
  appendVec(request.buf_, "POST /index.html?a HTTP/1.1\r\nHost: ");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "ama\r\nlanguage: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "en-US\r\nTransfer-Encoding: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "Chunked\r\n\r\n012345678");
  EXPECT_EQ(request.parseRequest(*session), REQ_CONTINUE_RECV);
  appendVec(request.buf_, "9abcdefghij");
  EXPECT_EQ(request.method_, "POST");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.query_, "a");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
  EXPECT_EQ(request.headers_["transfer-encoding"], "chunked");
}

TEST_F(test_parseRequest, transferEncodingNG1) {
  appendVec(request.buf_, "POST /index.html?a HTTP/1.1\r\nHost: ");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "ama\r\nlanguage: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "en-US\r\nTransfer-Encoding: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "hunked\r\n\r\n012345678");
  EXPECT_EQ(request.parseRequest(*session), REQ_ERR_LEN_REQUIRED);
  appendVec(request.buf_, "9abcdefghij");
  EXPECT_EQ(request.method_, "POST");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.query_, "a");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
  EXPECT_EQ(request.headers_["transfer-encoding"], "hunked");
}

TEST_F(test_parseRequest, transferEncodingNG2) {
  appendVec(request.buf_, "POST /index.html?a HTTP/1.1\r\nHost: ");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "ama\r\nlanguage: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "en-US\r\nTransfer-Encoding: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "chunked\r\n\r\nZZZ\r\n");
  EXPECT_EQ(request.parseRequest(*session), REQ_ERR_BAD_REQUEST);
  EXPECT_EQ(request.method_, "POST");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.query_, "a");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
  EXPECT_EQ(request.headers_["transfer-encoding"], "chunked");
}

TEST_F(test_parseRequest, transferEncodingOK2) {
  appendVec(request.buf_, "POST /index.html?a HTTP/1.1\r\nHost: ");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "ama\r\nlanguage: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "en-US\r\nTransfer-Encoding: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "chunked\r\n\r\n5\r\n01234\r\n0\r\n\r\n");
  EXPECT_EQ(request.parseRequest(*session), REQ_FIN_RECV);
  EXPECT_EQ(request.method_, "POST");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.query_, "a");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
  EXPECT_EQ(request.headers_["transfer-encoding"], "chunked");
  std::string str(request.body_.begin(), request.body_.end());
  EXPECT_EQ(str, "01234");
}

TEST_F(test_parseRequest, transferEncodingOK3) {
  appendVec(request.buf_, "POST /index.html?a HTTP/1.1\r\nHost: ");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "ama\r\nlanguage: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "en-US\r\nTransfer-Encoding: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "chunked\r\n\r\n5\r\n01234\r\n3");
  EXPECT_EQ(request.parseRequest(*session), REQ_CONTINUE_RECV);
  EXPECT_EQ(request.method_, "POST");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.query_, "a");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
  EXPECT_EQ(request.headers_["transfer-encoding"], "chunked");
  appendVec(request.buf_, "\r\n567\r\n0\r\n\r\n");
  EXPECT_EQ(request.parseRequest(*session), REQ_FIN_RECV);
  std::string str(request.body_.begin(), request.body_.end());
  EXPECT_EQ(str, "01234567");
}

TEST_F(test_parseRequest, transferEncodingNG3) {
  appendVec(request.buf_, "POST /index.html?a HTTP/1.1\r\nHost: ");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "ama\r\nlanguage: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "en-US\r\nTransfer-Encoding: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "chunked\r\n\r\n5\r\n01234\r\nA");
  EXPECT_EQ(request.parseRequest(*session), REQ_CONTINUE_RECV);
  EXPECT_EQ(request.method_, "POST");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.query_, "a");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
  EXPECT_EQ(request.headers_["transfer-encoding"], "chunked");
  appendVec(request.buf_, "\r\n0123456789ab\r\n");
  EXPECT_EQ(request.parseRequest(*session), REQ_ERR_BAD_REQUEST);
  std::string str(request.body_.begin(), request.body_.end());
  EXPECT_EQ(str, "01234");
}

TEST_F(test_parseRequest, transferEncodingOK4) {
  appendVec(request.buf_, "POST /index.html?a HTTP/1.1\r\nHost: ");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "ama\r\nlanguage: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "en-US\r\nTransfer-Encoding: \t\t");
  EXPECT_EQ(request.parseRequest(*session), 1);
  appendVec(request.buf_, "chunked\r\n\r\n5\r\n01234\r\n3");
  EXPECT_EQ(request.parseRequest(*session), REQ_CONTINUE_RECV);
  EXPECT_EQ(request.method_, "POST");
  EXPECT_EQ(request.uri_, "/index.html");
  EXPECT_EQ(request.query_, "a");
  EXPECT_EQ(request.headers_["host"], "localhost");
  EXPECT_EQ(request.headers_["location"], "Yokohama");
  EXPECT_EQ(request.headers_["language"], "en-US");
  EXPECT_EQ(request.headers_["transfer-encoding"], "chunked");
  appendVec(request.buf_, "\r\n567\r\nFF\r\n123456789abcdef");
  EXPECT_EQ(request.parseRequest(*session), REQ_CONTINUE_RECV);
  appendVec(request.buf_, "\r\n0\r\n\r\n");
  EXPECT_EQ(request.parseRequest(*session), REQ_FIN_RECV);
  std::string str(request.body_.begin(), request.body_.end());
  EXPECT_EQ(str, "01234567123456789abcdef");
}

TEST_F(test_parseRequest, RequestOnceOK) {
  const_cast<MainConfig &>(session->main_config_).client_max_body_size_ = 100;
  appendVec(
      request.buf_,
      "POST /index.html?a "
      "HTTP/"
      "1.1\r\nHost:localhost\r\nContent-length: 10\r\n\r\n0123456789\r\n");
  EXPECT_EQ(0, request.parseRequest(*session));
  std::string str(request.body_.begin(), request.body_.end());
  EXPECT_EQ("0123456789", str);
}

TEST_F(test_parseRequest, RequestNGtooLarge1) {
  const_cast<MainConfig &>(session->main_config_).client_max_body_size_ = 100;
  appendVec(
      request.buf_,
      "POST /index.html?a "
      "HTTP/"
      "1.1\r\nHost:localhost\r\nContent-length: 101\r\n\r\n0123456789\r\n");
  EXPECT_EQ(100, session->getClientMaxBodySize());
  EXPECT_EQ(REQ_ERR_TOO_LARGE, request.parseRequest(*session));
}

TEST_F(test_parseRequest, RequestNGtooLarge2) {
  const_cast<MainConfig &>(session->main_config_).client_max_body_size_ = 100;
  session->server_config_ = new ServerConfig;
  const_cast<ServerConfig *>(session->server_config_)
      ->flg_client_max_body_size_set_ = true;
  const_cast<ServerConfig *>(session->server_config_)->client_max_body_size_ =
      50;
  EXPECT_EQ(50, session->getClientMaxBodySize());
  appendVec(
      request.buf_,
      "POST /index.html?a "
      "HTTP/"
      "1.1\r\nHost:localhost\r\nContent-length: 51\r\n\r\n0123456789\r\n");
  EXPECT_EQ(REQ_ERR_TOO_LARGE, request.parseRequest(*session));
}

TEST_F(test_parseRequest, RequestNGtooLarge3) {
  const_cast<MainConfig &>(session->main_config_).client_max_body_size_ = 100;
  session->server_config_ = new ServerConfig;
  const_cast<ServerConfig *>(session->server_config_)
      ->flg_client_max_body_size_set_ = true;
  const_cast<ServerConfig *>(session->server_config_)->client_max_body_size_ =
      50;
  session->location_config_ = new LocationConfig;
  const_cast<LocationConfig *>(session->location_config_)
      ->flg_client_max_body_size_set_ = true;
  const_cast<LocationConfig *>(session->location_config_)
      ->client_max_body_size_ = 42;
  EXPECT_EQ(42, session->getClientMaxBodySize());
  appendVec(
      request.buf_,
      "POST /index.html?a "
      "HTTP/"
      "1.1\r\nHost:localhost\r\nContent-length: 43\r\n\r\n0123456789\r\n");
  EXPECT_EQ(REQ_ERR_TOO_LARGE, request.parseRequest(*session));
}