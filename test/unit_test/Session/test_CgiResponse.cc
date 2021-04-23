/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_CgiArgvEnvp.cc                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/04/02 14:21:46 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "MainConfig.hpp"
#include "Session.hpp"
#include "gtest.h"
#include "webserv_settings.hpp"
#include "CgiParams.hpp"

class test_CgiResponse : public ::testing::Test {
 protected:
  MainConfig config;
  Session* session;
  bool flg_thrown;
  CgiParams* cgi_params;

  void appendVec(std::vector<char>& vec, const std::string& str) {
    vec.insert(vec.end(), str.begin(), str.end());
  }

  std::string vecToString(std::vector<char>& buf, size_t begin, size_t end) {
    std::string ret;
    for (size_t i = begin; i != end; ++i) {
      ret += buf[i];
    }
    return ret;
  }

  virtual void SetUp() {
    flg_thrown = false;
    session = new Session(0, config);
    session->ip_ = 16777343;  // 127.0.0.1
    session->port_ = 47138;   // 8888
    config.root_ = "./html";
    cgi_params = new CgiParams(*session);
  }

  // 各ケース共通の後処理を書く
  virtual void TearDown() { delete session; }
};

TEST_F(test_CgiResponse, headerOK1) {
  ssize_t end_header = session->parseReadBuf("Content-Type: text/html\n\n", 26);
  EXPECT_EQ(24, end_header);
  EXPECT_EQ("HTTP/1.1 200 OK\r\nServer: nginDX\r\nContent-Type: text/html\r\n",
            session->response_.status_header_);
}

TEST_F(test_CgiResponse, headerOK2) {
  ssize_t end_header =
      session->parseReadBuf("Content-Type: text/html\nhoge: foo\n\n", 36);
  EXPECT_EQ(34, end_header);
  EXPECT_EQ(
      "HTTP/1.1 200 OK\r\nServer: nginDX\r\nContent-Type: text/html\r\nhoge: "
      "foo\r\n",
      session->response_.status_header_);
}

TEST_F(test_CgiResponse, headerNG1) {
  EXPECT_EQ(-1,
            session->parseReadBuf("Content-T: text/html\nhoge: foo\n\n", 36));
  EXPECT_EQ("", session->response_.status_header_);
}

TEST_F(test_CgiResponse, headerNG2) {
  EXPECT_EQ(-1, session->parseReadBuf("\n\n", 3));
  EXPECT_EQ("", session->response_.status_header_);
}

TEST_F(test_CgiResponse, BodyOK1) {
  std::string buf("Content-Type: text/html\n\n<HTML>\n");
  ssize_t end_header = session->parseReadBuf("Content-Type: text/html\n\n", 26);
  EXPECT_EQ(24, end_header);
  EXPECT_EQ("HTTP/1.1 200 OK\r\nServer: nginDX\r\nContent-Type: text/html\r\n",
            session->response_.status_header_);
  session->response_.appendToBody(buf.c_str() + end_header + 1,
                                  buf.size() - (end_header + 1));
  EXPECT_EQ("<HTML>\n", vecToString(session->response_.body_, 0,
                                    buf.size() - (end_header + 1)));
}

TEST_F(test_CgiResponse, StatusCodeChangeOK1) {
  std::string buf("Status: 777 Lucky\n\n<HTML>\n");
  ssize_t end_header = session->parseReadBuf(buf.c_str(), 27);
  EXPECT_EQ(18, end_header);
  EXPECT_EQ("HTTP/1.1 777 Lucky\r\nServer: nginDX\r\n",
            session->response_.status_header_);
  session->response_.appendToBody(buf.c_str() + end_header + 1,
                                  buf.size() - (end_header + 1));
  EXPECT_EQ("<HTML>\n", vecToString(session->response_.body_, 0,
                                    buf.size() - (end_header + 1)));
}
