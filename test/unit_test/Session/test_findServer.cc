/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_findServer.cc                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/29 18:07:58 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "MainConfig.hpp"
#include "Session.hpp"
#include "gtest.h"

class test_findServer : public ::testing::Test {
 protected:
  MainConfig config;
  ServerConfig server[5];
  Session* session;
  bool flg_thrown;

  virtual void SetUp() {
    flg_thrown = false;
    for (int i = 0; i < 5; i++) {
      config.addServer(server[i]);
    }
    session = new Session(0, config);
    session->request_.headers_["host"] = "localhost:8080";
  }

  // 各ケース共通の後処理を書く
  virtual void TearDown() { delete session; }
};

TEST_F(test_findServer, findFromOne) {
  const ServerConfig* res;
  server[0].addToListen(0x12345678, 0x1234);
  server[0].server_name_.push_back("SelectMe");
  config.addServer(server[0]);

  res = session->findServer();
  EXPECT_EQ(res->server_name_.front(), "SelectMe");
}

TEST_F(test_findServer, findFromOneADDRANY) {
  const ServerConfig* res;
  server[0].addToListen(INADDR_ANY, 0x1234);
  server[0].server_name_.push_back("SelectMe");
  config.addServer(server[0]);

  res = session->findServer();
  EXPECT_EQ(res->server_name_.front(), "SelectMe");
}

TEST_F(test_findServer, findFromOneManyListen) {
  const ServerConfig* res;
  server[0].addToListen(INADDR_ANY, 0x4321);
  server[0].addToListen(0x87654321, 0x1234);
  server[0].addToListen(0x87654321, 0x4321);
  server[0].addToListen(0x12345678, 0x1234);
  server[0].addToListen(0x12345678, 0x1111);
  server[0].addToListen(0x11111111, 0x1234);
  server[0].server_name_.push_back("SelectMe");
  config.addServer(server[0]);

  res = session->findServer();
  EXPECT_EQ(res->server_name_.front(), "SelectMe");
}

TEST_F(test_findServer, findFromThreeNormal1) {
  const ServerConfig* res;
  server[0].addToListen(0x42424242, 0x4242);
  server[0].addToListen(0x4224242, 0x422);
  server[0].addToListen(0x4244242, 0x242);
  server[0].addToListen(0x12345678, 0x1234);
  server[0].addToListen(0x4244242, 0x242);
  server[0].addToListen(0x4242442, 0x4242);
  server[0].server_name_.push_back("SelectMe");
  config.addServer(server[0]);
  server[1].addToListen(0x42424242, 0x4242);
  server[1].addToListen(0x4224242, 0x422);
  server[1].addToListen(0x4244242, 0x242);
  server[1].addToListen(0x4244242, 0x242);
  server[1].addToListen(0x4242442, 0x4242);
  server[1].addToListen(0x87654321, 0x1234);
  config.addServer(server[1]);
  server[2].addToListen(INADDR_ANY, 0x4321);
  server[2].addToListen(0x42424242, 0x4242);
  server[2].addToListen(0x4224242, 0x422);
  server[2].addToListen(0x4244242, 0x242);
  server[2].addToListen(0x4244242, 0x242);
  server[2].addToListen(0x4242442, 0x4242);
  config.addServer(server[2]);
  server[3].addToListen(0x11111111, 0x1234);
  server[3].addToListen(0x4224242, 0x422);
  server[3].addToListen(0x4244242, 0x242);
  server[3].addToListen(0x4244242, 0x242);
  config.addServer(server[3]);
  server[4].addToListen(0x12345678, 0x1111);
  server[4].addToListen(0x4224242, 0x422);
  server[4].addToListen(0x4244242, 0x242);
  server[4].addToListen(0x4244242, 0x242);
  config.addServer(server[4]);

  res = session->findServer();
  EXPECT_EQ(res->server_name_.front(), "SelectMe");
}

TEST_F(test_findServer, findFromThreeNormal2) {
  const ServerConfig* res;
  server[0].addToListen(INADDR_ANY, 0x4321);
  server[0].addToListen(0x42424242, 0x4242);
  server[0].addToListen(0x4224242, 0x422);
  server[0].addToListen(0x4244242, 0x242);
  config.addServer(server[0]);
  server[1].addToListen(0x42424242, 0x4242);
  server[1].addToListen(0x4224242, 0x422);
  server[1].addToListen(0x4244242, 0x242);
  server[1].addToListen(0x87654321, 0x1234);
  config.addServer(server[1]);
  server[2].addToListen(0x42424242, 0x4242);
  server[2].addToListen(0x12345678, 0x1234);
  server[2].addToListen(0x4244242, 0x242);
  server[2].server_name_.push_back("SelectMe");
  config.addServer(server[2]);
  server[3].addToListen(0x4224242, 0x422);
  server[3].addToListen(0x4244242, 0x242);
  server[3].addToListen(0x11111111, 0x1234);
  config.addServer(server[3]);
  server[4].addToListen(0x4244242, 0x242);
  server[4].addToListen(0x4244242, 0x242);
  server[4].addToListen(0x12345678, 0x1111);
  config.addServer(server[4]);

  res = session->findServer();
  EXPECT_EQ(res->server_name_.front(), "SelectMe");
}

TEST_F(test_findServer, findFromThreeNormal3) {
  const ServerConfig* res;
  server[0].addToListen(0x4244242, 0x242);
  server[0].addToListen(0x4244242, 0x242);
  server[0].addToListen(0x4242442, 0x4242);
  server[0].addToListen(0x12345678, 0x1111);
  config.addServer(server[0]);
  server[1].addToListen(0x87654321, 0x1234);
  server[1].addToListen(0x42424242, 0x4242);
  server[1].addToListen(0x4224242, 0x422);
  server[1].addToListen(0x4244242, 0x242);
  config.addServer(server[1]);
  server[2].addToListen(INADDR_ANY, 0x4321);
  server[2].addToListen(INADDR_ANY, 0xa21);
  server[2].addToListen(INADDR_ANY, 0x321);
  server[2].addToListen(INADDR_ANY, 0x4351);
  server[2].addToListen(INADDR_ANY, 0x4421);
  config.addServer(server[2]);
  server[3].addToListen(0x11111111, 0x1234);
  config.addServer(server[3]);
  server[4].addToListen(0x12345678, 0x1234);
  server[4].addToListen(0x4244242, 0x242);
  server[4].addToListen(0x4244242, 0x242);
  server[4].server_name_.push_back("SelectMe");
  config.addServer(server[4]);

  res = session->findServer();
  EXPECT_EQ(res->server_name_.front(), "SelectMe");
}

TEST_F(test_findServer, findFromFiveDuplicateIPPort) {
  const ServerConfig* res;
  server[0].addToListen(0x12345678, 0x1234);
  server[0].server_name_.push_back("SelectMe");
  config.addServer(server[0]);
  server[1].addToListen(0x12345678, 0x1234);
  config.addServer(server[1]);
  server[2].addToListen(0x12345678, 0x1234);
  config.addServer(server[2]);
  server[3].addToListen(0x12345678, 0x1234);
  config.addServer(server[3]);
  server[4].addToListen(0x12345678, 0x1234);
  config.addServer(server[4]);

  res = session->findServer();
  EXPECT_EQ(res->server_name_.front(), "SelectMe");
}

TEST_F(test_findServer, findFromFiveDuplicateANY) {
  const ServerConfig* res;
  server[0].addToListen(INADDR_ANY, 0x1234);
  server[0].server_name_.push_back("SelectMe");
  config.addServer(server[0]);
  server[1].addToListen(0x12345678, 0x1234);
  config.addServer(server[1]);

  res = session->findServer();
  EXPECT_EQ(res->server_name_.front(), "SelectMe");
}

TEST_F(test_findServer, findFromFiveDuplicateANY2) {
  const ServerConfig* res;
  server[0].addToListen(0x12345678, 0x1234);
  server[0].server_name_.push_back("SelectMe");
  config.addServer(server[0]);
  server[1].addToListen(INADDR_ANY, 0x1234);
  config.addServer(server[1]);

  res = session->findServer();
  EXPECT_EQ(res->server_name_.front(), "SelectMe");
}

TEST_F(test_findServer, findFromFiveDuplicateANYwithServerName) {
  const ServerConfig* res;
  server[0].addToListen(INADDR_ANY, 0x1234);
  server[0].server_name_.push_back("SelectMe");
  server[0].server_name_.push_back("localhost");
  config.addServer(server[0]);
  server[1].addToListen(0x12345678, 0x1234);
  server[1].server_name_.push_back("DontSelectMe");
  config.addServer(server[1]);

  res = session->findServer();
  EXPECT_EQ(res->server_name_.front(), "SelectMe");
}

TEST_F(test_findServer, findFromFiveDuplicateANYwithServerName2) {
  const ServerConfig* res;
  server[0].addToListen(INADDR_ANY, 0x1234);
  server[0].server_name_.push_back("DontSelectMe");
  config.addServer(server[0]);
  server[1].addToListen(INADDR_ANY, 0x1234);
  server[1].server_name_.push_back("SelectMe");
  server[1].server_name_.push_back("localhost");
  config.addServer(server[1]);

  res = session->findServer();
  EXPECT_EQ(res->server_name_.front(), "SelectMe");
}
