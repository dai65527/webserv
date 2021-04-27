/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_getUriFromLocation.cc                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/04/27 08:18:48 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "MainConfig.hpp"
#include "Session.hpp"
#include "gtest.h"

class test_getUriFromLocation : public ::testing::Test {
 protected:
  MainConfig config;
  ServerConfig server;
  LocationConfig location;
  Session* session;
  bool flg_thrown;

  virtual void SetUp() {
    flg_thrown = false;
    server.addLocation(location);
    config.addServer(server);
    session = new Session(0, config);
    session->server_config_ = &server;
    session->location_config_ = &location;
  }

  // 各ケース共通の後処理を書く
  virtual void TearDown() { delete session; }
};

TEST_F(test_getUriFromLocation, locationRoot) {
  std::string res;
  location.route_ = "/";
  session->request_.uri_ = "/hoge";
  res = session->getUriFromLocation();

  EXPECT_EQ(res, "/hoge");
}

TEST_F(test_getUriFromLocation, locationDirectoryWithSlash) {
  std::string res;
  location.route_ = "/directory/";
  session->request_.uri_ = "/directory/hoge";
  res = session->getUriFromLocation();

  EXPECT_EQ(res, "/hoge");
}

TEST_F(test_getUriFromLocation, locationDirectoryWithOutSlash) {
  std::string res;
  location.route_ = "/directory";
  session->request_.uri_ = "/directory/hoge";
  res = session->getUriFromLocation();

  EXPECT_EQ(res, "/hoge");
}
