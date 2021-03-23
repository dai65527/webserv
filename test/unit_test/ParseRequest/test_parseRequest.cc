/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parseRequest.cc                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 01:10:10 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/24 03:50:58 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "gtest.h"
#include "Request.hpp"

class test_parseRequest : public ::testing::Test {
 protected:
  bool flg_thrown = false;
  Request request;

  // 各ケース共通の前処理を書く
  virtual void SetUp() {}

  // 各ケース共通の後処理を書く
  virtual void TearDown() {}
};

TEST_F(test_parseRequest, allOK) {
  request.buf_ = "GET / HTTP/1.1\r\n";
  EXPECT_NO_THROW(request.parseRequest());
  EXPECT_EQ(request.method_, "GET");
  EXPECT_EQ(request.uri_, "/");
}

TEST_F(test_parseRequest, allOK2) {
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

// TEST_F(test_parseRequest, failMethod) {
//   request.buf_ = " GET / HTTP/1.1\r\n";
//   EXPECT_NO_THROW(request.parseRequest());
//   EXPECT_EQ(request.method_, "GET");
//   EXPECT_EQ(request.uri_, "/");
// }

// TEST_F(test_parseRequest, uri) {
//   request.buf_ = "GET / HTTP/1.1";
//   request.parseRequest();
  
// }

// TEST_F(test_parseCgiExtension, normalThree) {
//   settings.push_back("cgi");
//   settings.push_back("php");
//   settings.push_back("perl");
//   EXPECT_NO_THROW(store.parseCgiExtension(settings));
//   EXPECT_EQ(store.getCgiExtension(), settings);
// }

// TEST_F(test_parseCgiExtension, normalOneTwice) {
//   settings.push_back("cgi");
//   EXPECT_NO_THROW(store.parseCgiExtension(settings));
//   settings.front() = "perl";
//   EXPECT_NO_THROW(store.parseCgiExtension(settings));
//   std::list<std::string>::const_iterator itr = store.getCgiExtension().begin();
//   EXPECT_EQ(*itr, "cgi");
//   EXPECT_EQ(*++itr, "perl");
// }

// TEST_F(test_parseCgiExtension, normalOneTwice3times) {
//   settings.push_back("cgi");
//   EXPECT_NO_THROW(store.parseCgiExtension(settings));
//   settings.front() = "perl";
//   EXPECT_NO_THROW(store.parseCgiExtension(settings));
//   settings.front() = "php";
//   EXPECT_NO_THROW(store.parseCgiExtension(settings));
//   std::list<std::string>::const_iterator itr = store.getCgiExtension().begin();
//   EXPECT_EQ(*itr, "cgi");
//   EXPECT_EQ(*++itr, "perl");
//   EXPECT_EQ(*++itr, "php");
// }

// TEST_F(test_parseCgiExtension, settingInvalidNumber) {
//   try {
//     store.parseCgiExtension(settings);
//   } catch (const std::runtime_error& e) {
//     EXPECT_STREQ(e.what(), "cgi_extension: invalid number of setting");
//     flg_thrown = true;
//   }
//   EXPECT_TRUE(flg_thrown);
// }