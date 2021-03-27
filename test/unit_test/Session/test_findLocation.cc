/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_findLocation.cc                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/26 12:35:37 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "MainConfig.hpp"
#include "Session.hpp"
#include "gtest.h"

class test_findLocation : public ::testing::Test {
 protected:
  MainConfig config;
  ServerConfig server;
  Session* session;
  LocationConfig loc[5];
  bool flg_thrown;

  virtual void SetUp() {
    flg_thrown = false;
    loc[2].setRoute("/fuga/");
    loc[3].setRoute("/hoge/fuga/");
    loc[0].setRoute("/");
    loc[1].setRoute("/hoge/");
    loc[4].setRoute("/hoge/hoge/");
    for (int i = 0; i < 5; i++) {
      server.addLocation(loc[i]);
    }
    config.addServer(server);
    session = new Session(0, config);
    session->server_config_ = &config.getServers().front();
  }

  // 各ケース共通の後処理を書く
  virtual void TearDown() { delete session; }
};

TEST_F(test_findLocation, pathFullMatch1) {
  const LocationConfig* res;
  session->request_.uri_ = "/";
  res = session->findLocation();
  EXPECT_EQ(res->getRoute(), "/");
}

TEST_F(test_findLocation, pathFullMatch2) {
  const LocationConfig* res;
  session->request_.uri_ = "/hoge/";
  res = session->findLocation();
  EXPECT_EQ(res->getRoute(), "/hoge/");
}

TEST_F(test_findLocation, pathFullMatch3) {
  const LocationConfig* res;
  session->request_.uri_ = "/fuga/";
  res = session->findLocation();
  EXPECT_EQ(res->getRoute(), "/fuga/");
}

TEST_F(test_findLocation, pathFullMatch4) {
  const LocationConfig* res;
  session->request_.uri_ = "/hoge/fuga/";
  res = session->findLocation();
  EXPECT_EQ(res->getRoute(), "/hoge/fuga/");
}

TEST_F(test_findLocation, pathFullMatch5) {
  const LocationConfig* res;
  session->request_.uri_ = "/hoge/hoge/";
  res = session->findLocation();
  EXPECT_EQ(res->getRoute(), "/hoge/hoge/");
}

TEST_F(test_findLocation, partialMatch) {
  const LocationConfig* res;
  session->request_.uri_ = "/foo";
  res = session->findLocation();
  EXPECT_EQ(res->getRoute(), "/");
}

TEST_F(test_findLocation, partialMatch2) {
  const LocationConfig* res;
  session->request_.uri_ = "/foo/bar/";
  res = session->findLocation();
  EXPECT_EQ(res->getRoute(), "/");
}

TEST_F(test_findLocation, pathPartialMatch3) {
  const LocationConfig* res;
  session->request_.uri_ = "/hoge/foo/bar";
  res = session->findLocation();
  EXPECT_EQ(res->getRoute(), "/hoge/");
}

TEST_F(test_findLocation, pathPartialMatch4) {
  const LocationConfig* res;
  session->request_.uri_ = "/fuga/foo/bar";
  res = session->findLocation();
  EXPECT_EQ(res->getRoute(), "/fuga/");
}

TEST_F(test_findLocation, pathPartialMatch5) {
  const LocationConfig* res;
  session->request_.uri_ = "/hoge/fuga/foo/bar/";
  res = session->findLocation();
  EXPECT_EQ(res->getRoute(), "/hoge/fuga/");
}

TEST_F(test_findLocation, pathPartialMatch6) {
  const LocationConfig* res;
  session->request_.uri_ = "/hoge/hoge/foo/ber/hoge/";
  res = session->findLocation();
  EXPECT_EQ(res->getRoute(), "/hoge/hoge/");
}

TEST_F(test_findLocation, noMatch) {
  const LocationConfig* res;
  session->request_.uri_ = "hogehoge";
  res = session->findLocation();
  EXPECT_TRUE(res == NULL);
}
