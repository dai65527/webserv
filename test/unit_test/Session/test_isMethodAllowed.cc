/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_isMethodAllowed.cc                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/28 17:07:50 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "MainConfig.hpp"
#include "Session.hpp"
#include "gtest.h"

class test_isMethodAllowed : public ::testing::Test {
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

TEST_F(test_isMethodAllowed, allowedMainConfigGetHead) {
  config.limit_except_ = HTTP_GET | HTTP_HEAD;
  EXPECT_TRUE(session->isMethodAllowed(HTTP_GET));
  EXPECT_TRUE(session->isMethodAllowed(HTTP_HEAD));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_POST));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_PUT));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_DELETE));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_OPTIONS));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_TRACE));
}

TEST_F(test_isMethodAllowed, allowedMainConfigPost) {
  config.limit_except_ = HTTP_POST;
  EXPECT_FALSE(session->isMethodAllowed(HTTP_GET));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_HEAD));
  EXPECT_TRUE(session->isMethodAllowed(HTTP_POST));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_PUT));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_DELETE));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_OPTIONS));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_TRACE));
}

TEST_F(test_isMethodAllowed, allowedMainConfigPut) {
  config.limit_except_ = HTTP_PUT;
  EXPECT_FALSE(session->isMethodAllowed(HTTP_GET));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_HEAD));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_POST));
  EXPECT_TRUE(session->isMethodAllowed(HTTP_PUT));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_DELETE));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_OPTIONS));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_TRACE));
}

TEST_F(test_isMethodAllowed, allowedMainConfigDelete) {
  config.limit_except_ = HTTP_DELETE;
  EXPECT_FALSE(session->isMethodAllowed(HTTP_GET));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_HEAD));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_POST));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_PUT));
  EXPECT_TRUE(session->isMethodAllowed(HTTP_DELETE));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_OPTIONS));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_TRACE));
}

TEST_F(test_isMethodAllowed, allowedMainConfigOptions) {
  config.limit_except_ = HTTP_OPTIONS;
  EXPECT_FALSE(session->isMethodAllowed(HTTP_GET));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_HEAD));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_POST));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_PUT));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_DELETE));
  EXPECT_TRUE(session->isMethodAllowed(HTTP_OPTIONS));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_TRACE));
}

TEST_F(test_isMethodAllowed, allowedMainConfigTrace) {
  config.limit_except_ = HTTP_TRACE;
  EXPECT_FALSE(session->isMethodAllowed(HTTP_GET));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_HEAD));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_POST));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_PUT));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_DELETE));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_OPTIONS));
  EXPECT_TRUE(session->isMethodAllowed(HTTP_TRACE));
}

/*** server config ***/

TEST_F(test_isMethodAllowed, allowedServerConfigGetHead) {
  config.limit_except_ = 127; // allow all
  ServerConfig server_config;
  server_config.limit_except_ = HTTP_GET | HTTP_HEAD;
  session->server_config_ = &server_config;
  EXPECT_TRUE(session->isMethodAllowed(HTTP_GET));
  EXPECT_TRUE(session->isMethodAllowed(HTTP_HEAD));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_POST));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_PUT));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_DELETE));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_OPTIONS));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_TRACE));
}

TEST_F(test_isMethodAllowed, allowedServerConfigPost) {
  config.limit_except_ = 127; // allow all
  ServerConfig server_config;
  server_config.limit_except_ = HTTP_POST;
  session->server_config_ = &server_config;
  EXPECT_FALSE(session->isMethodAllowed(HTTP_GET));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_HEAD));
  EXPECT_TRUE(session->isMethodAllowed(HTTP_POST));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_PUT));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_DELETE));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_OPTIONS));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_TRACE));
}

/*** location config **/

TEST_F(test_isMethodAllowed, allowedLocationConfigGetHead) {
  config.limit_except_ = 127; // allow all
  ServerConfig server_config;
  server_config.limit_except_ = 127;
  LocationConfig location_config;
  location_config.limit_except_ = HTTP_GET | HTTP_HEAD;
  session->server_config_ = &server_config;
  session->location_config_ = &location_config;
  EXPECT_TRUE(session->isMethodAllowed(HTTP_GET));
  EXPECT_TRUE(session->isMethodAllowed(HTTP_HEAD));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_POST));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_PUT));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_DELETE));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_OPTIONS));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_TRACE));
}

TEST_F(test_isMethodAllowed, allowedLocationConfigPut) {
  config.limit_except_ = 127; // allow all
  ServerConfig server_config;
  server_config.limit_except_ = 127;
  LocationConfig location_config;
  location_config.limit_except_ = HTTP_PUT;
  session->server_config_ = &server_config;
  session->location_config_ = &location_config;
  EXPECT_FALSE(session->isMethodAllowed(HTTP_GET));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_HEAD));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_POST));
  EXPECT_TRUE(session->isMethodAllowed(HTTP_PUT));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_DELETE));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_OPTIONS));
  EXPECT_FALSE(session->isMethodAllowed(HTTP_TRACE));
}
