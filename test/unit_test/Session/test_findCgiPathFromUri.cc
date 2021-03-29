/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_findCgiPathFromUri.cc                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/28 18:00:31 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "MainConfig.hpp"
#include "Session.hpp"
#include "gtest.h"

class test_cgiPathFromUri : public ::testing::Test {
 protected:
  MainConfig config;
  Session* session;
  bool flg_thrown;

  virtual void SetUp() {
    flg_thrown = false;
    session = new Session(0, config);
  }

  // 各ケース共通の後処理を書く
  virtual void TearDown() { delete session; }
};

TEST_F(test_cgiPathFromUri, mainConfig1) {
  config.cgi_extension_.push_back("cgi");
  config.cgi_extension_.push_back("php");
  config.cgi_extension_.push_back("sh");
  session->request_.uri_ = "/hoge/fuga/fuga.cgi/";
  EXPECT_EQ(session->findCgiPathFromUri(), "/hoge/fuga/fuga.cgi");
}

TEST_F(test_cgiPathFromUri, mainConfig2) {
  config.cgi_extension_.push_back("cgi");
  config.cgi_extension_.push_back("php");
  config.cgi_extension_.push_back("sh");
  session->request_.uri_ = "/hoge/fuga/fuga.cgi";
  EXPECT_EQ(session->findCgiPathFromUri(), "/hoge/fuga/fuga.cgi");
}

TEST_F(test_cgiPathFromUri, mainConfig3) {
  config.cgi_extension_.push_back("cgi");
  config.cgi_extension_.push_back("php");
  config.cgi_extension_.push_back("sh");
  session->request_.uri_ = "/hoge/fuga.cgi/fuga";
  EXPECT_EQ(session->findCgiPathFromUri(), "/hoge/fuga.cgi");
}

TEST_F(test_cgiPathFromUri, mainConfig4) {
  config.cgi_extension_.push_back("cgi");
  config.cgi_extension_.push_back("php");
  config.cgi_extension_.push_back("sh");
  session->request_.uri_ = "/hoge.cgi/fuga/fuga";
  EXPECT_EQ(session->findCgiPathFromUri(), "/hoge.cgi");
}

TEST_F(test_cgiPathFromUri, mainConfig5) {
  config.cgi_extension_.push_back("php");
  config.cgi_extension_.push_back("cgi");
  config.cgi_extension_.push_back("sh");
  session->request_.uri_ = "/hoge/fuga/fuga.cgi/";
  EXPECT_EQ(session->findCgiPathFromUri(), "/hoge/fuga/fuga.cgi");
}

TEST_F(test_cgiPathFromUri, mainConfig6) {
  config.cgi_extension_.push_back("php");
  config.cgi_extension_.push_back("cgi");
  config.cgi_extension_.push_back("sh");
  session->request_.uri_ = "/hoge/fuga/fuga.cgi";
  EXPECT_EQ(session->findCgiPathFromUri(), "/hoge/fuga/fuga.cgi");
}

TEST_F(test_cgiPathFromUri, mainConfig7) {
  config.cgi_extension_.push_back("php");
  config.cgi_extension_.push_back("sh");
  config.cgi_extension_.push_back("cgi");
  session->request_.uri_ = "/hoge/fuga.cgi/fuga";
  EXPECT_EQ(session->findCgiPathFromUri(), "/hoge/fuga.cgi");
}

TEST_F(test_cgiPathFromUri, mainConfig8) {
  config.cgi_extension_.push_back("php");
  config.cgi_extension_.push_back("sh");
  config.cgi_extension_.push_back("cgi");
  session->request_.uri_ = "/hoge.cgi/fuga/fuga";
  EXPECT_EQ(session->findCgiPathFromUri(), "/hoge.cgi");
}

TEST_F(test_cgiPathFromUri, serverConfig1) {
  ServerConfig server_config;
  server_config.cgi_extension_.push_back("hoge");
  server_config.cgi_extension_.push_back("perl");
  server_config.cgi_extension_.push_back("fuga");
  session->server_config_ = &server_config;
  config.cgi_extension_.push_back("php");
  config.cgi_extension_.push_back("sh");
  session->request_.uri_ = "/hoge/fuga/fuga.perl/";
  EXPECT_EQ(session->findCgiPathFromUri(), "/hoge/fuga/fuga.perl");
}

TEST_F(test_cgiPathFromUri, serverConfig2) {
  ServerConfig server_config;
  server_config.cgi_extension_.push_back("hoge");
  server_config.cgi_extension_.push_back("perl");
  server_config.cgi_extension_.push_back("fuga");
  session->server_config_ = &server_config;
  config.cgi_extension_.push_back("php");
  config.cgi_extension_.push_back("sh");
  session->request_.uri_ = "/hoge/fuga/fuga.perl";
  EXPECT_EQ(session->findCgiPathFromUri(), "/hoge/fuga/fuga.perl");
}

TEST_F(test_cgiPathFromUri, serverConfig3) {
  ServerConfig server_config;
  server_config.cgi_extension_.push_back("hoge");
  server_config.cgi_extension_.push_back("perl");
  server_config.cgi_extension_.push_back("fuga");
  session->server_config_ = &server_config;
  config.cgi_extension_.push_back("php");
  config.cgi_extension_.push_back("sh");
  session->request_.uri_ = "/hoge/fuga.perl/fuga";
  EXPECT_EQ(session->findCgiPathFromUri(), "/hoge/fuga.perl");
}

TEST_F(test_cgiPathFromUri, serverConfig4) {
  ServerConfig server_config;
  server_config.cgi_extension_.push_back("hoge");
  server_config.cgi_extension_.push_back("perl");
  server_config.cgi_extension_.push_back("fuga");
  session->server_config_ = &server_config;
  config.cgi_extension_.push_back("php");
  config.cgi_extension_.push_back("sh");
  session->request_.uri_ = "/hoge.perl/fuga/fuga";
  EXPECT_EQ(session->findCgiPathFromUri(), "/hoge.perl");
}

TEST_F(test_cgiPathFromUri, serverConfig5) {
  ServerConfig server_config;
  server_config.cgi_extension_.push_back("hoge");
  server_config.cgi_extension_.push_back("perl");
  server_config.cgi_extension_.push_back("fuga");
  session->server_config_ = &server_config;
  config.cgi_extension_.push_back("php");
  config.cgi_extension_.push_back("sh");
  session->request_.uri_ = "/hoge/fuga/fuga.perl/";
  EXPECT_EQ(session->findCgiPathFromUri(), "/hoge/fuga/fuga.perl");
}

TEST_F(test_cgiPathFromUri, serverConfig6) {
  ServerConfig server_config;
  server_config.cgi_extension_.push_back("hoge");
  server_config.cgi_extension_.push_back("perl");
  server_config.cgi_extension_.push_back("fuga");
  session->server_config_ = &server_config;
  config.cgi_extension_.push_back("php");
  config.cgi_extension_.push_back("sh");
  session->request_.uri_ = "/hoge/fuga/fuga.perl";
  EXPECT_EQ(session->findCgiPathFromUri(), "/hoge/fuga/fuga.perl");
}

TEST_F(test_cgiPathFromUri, serverConfig7) {
  ServerConfig server_config;
  server_config.cgi_extension_.push_back("hoge");
  server_config.cgi_extension_.push_back("perl");
  server_config.cgi_extension_.push_back("fuga");
  session->server_config_ = &server_config;
  config.cgi_extension_.push_back("php");
  config.cgi_extension_.push_back("sh");
  session->request_.uri_ = "/hoge/fuga.perl/fuga";
  EXPECT_EQ(session->findCgiPathFromUri(), "/hoge/fuga.perl");
}

TEST_F(test_cgiPathFromUri, serverConfig8) {
  ServerConfig server_config;
  server_config.cgi_extension_.push_back("hoge");
  server_config.cgi_extension_.push_back("perl");
  server_config.cgi_extension_.push_back("fuga");
  session->server_config_ = &server_config;
  config.cgi_extension_.push_back("php");
  config.cgi_extension_.push_back("sh");
  session->request_.uri_ = "/hoge.perl/fuga/fuga";
  EXPECT_EQ(session->findCgiPathFromUri(), "/hoge.perl");
}

TEST_F(test_cgiPathFromUri, locationConfig1) {
  LocationConfig location_config;
  location_config.cgi_extension_.push_back("hoge");
  location_config.cgi_extension_.push_back("perl");
  location_config.cgi_extension_.push_back("fuga");
  session->location_config_ = &location_config;
  config.cgi_extension_.push_back("php");
  config.cgi_extension_.push_back("sh");
  session->request_.uri_ = "/hoge/fuga/fuga.perl/";
  EXPECT_EQ(session->findCgiPathFromUri(), "/hoge/fuga/fuga.perl");
}

TEST_F(test_cgiPathFromUri, locationConfig2) {
  LocationConfig location_config;
  location_config.cgi_extension_.push_back("hoge");
  location_config.cgi_extension_.push_back("perl");
  location_config.cgi_extension_.push_back("fuga");
  session->location_config_ = &location_config;
  config.cgi_extension_.push_back("php");
  config.cgi_extension_.push_back("sh");
  session->request_.uri_ = "/hoge/fuga/fuga.perl";
  EXPECT_EQ(session->findCgiPathFromUri(), "/hoge/fuga/fuga.perl");
}

TEST_F(test_cgiPathFromUri, locationConfig3) {
  LocationConfig location_config;
  location_config.cgi_extension_.push_back("hoge");
  location_config.cgi_extension_.push_back("perl");
  location_config.cgi_extension_.push_back("fuga");
  session->location_config_ = &location_config;
  config.cgi_extension_.push_back("php");
  config.cgi_extension_.push_back("sh");
  session->request_.uri_ = "/hoge/fuga.perl/fuga";
  EXPECT_EQ(session->findCgiPathFromUri(), "/hoge/fuga.perl");
}

TEST_F(test_cgiPathFromUri, locationConfig4) {
  LocationConfig location_config;
  location_config.cgi_extension_.push_back("hoge");
  location_config.cgi_extension_.push_back("perl");
  location_config.cgi_extension_.push_back("fuga");
  session->location_config_ = &location_config;
  config.cgi_extension_.push_back("php");
  config.cgi_extension_.push_back("sh");
  session->request_.uri_ = "/hoge.perl/fuga/fuga";
  EXPECT_EQ(session->findCgiPathFromUri(), "/hoge.perl");
}

TEST_F(test_cgiPathFromUri, locationConfig5) {
  LocationConfig location_config;
  location_config.cgi_extension_.push_back("hoge");
  location_config.cgi_extension_.push_back("perl");
  location_config.cgi_extension_.push_back("fuga");
  session->location_config_ = &location_config;
  config.cgi_extension_.push_back("php");
  config.cgi_extension_.push_back("sh");
  session->request_.uri_ = "/hoge/fuga/fuga.perl/";
  EXPECT_EQ(session->findCgiPathFromUri(), "/hoge/fuga/fuga.perl");
}

TEST_F(test_cgiPathFromUri, locationConfig6) {
  LocationConfig location_config;
  location_config.cgi_extension_.push_back("hoge");
  location_config.cgi_extension_.push_back("perl");
  location_config.cgi_extension_.push_back("fuga");
  session->location_config_ = &location_config;
  config.cgi_extension_.push_back("php");
  config.cgi_extension_.push_back("sh");
  session->request_.uri_ = "/hoge/fuga/fuga.perl";
  EXPECT_EQ(session->findCgiPathFromUri(), "/hoge/fuga/fuga.perl");
}

TEST_F(test_cgiPathFromUri, locationConfig7) {
  LocationConfig location_config;
  location_config.cgi_extension_.push_back("hoge");
  location_config.cgi_extension_.push_back("perl");
  location_config.cgi_extension_.push_back("fuga");
  session->location_config_ = &location_config;
  config.cgi_extension_.push_back("php");
  config.cgi_extension_.push_back("sh");
  session->request_.uri_ = "/hoge/fuga.perl/fuga";
  EXPECT_EQ(session->findCgiPathFromUri(), "/hoge/fuga.perl");
}

TEST_F(test_cgiPathFromUri, locationConfig8) {
  LocationConfig location_config;
  location_config.cgi_extension_.push_back("hoge");
  location_config.cgi_extension_.push_back("perl");
  location_config.cgi_extension_.push_back("fuga");
  session->location_config_ = &location_config;
  config.cgi_extension_.push_back("php");
  config.cgi_extension_.push_back("sh");
  session->request_.uri_ = "/hoge.perl/fuga/fuga";
  EXPECT_EQ(session->findCgiPathFromUri(), "/hoge.perl");
}
