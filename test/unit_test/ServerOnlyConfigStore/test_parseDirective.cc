/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parseDirective.cc                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/21 08:49:14 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerOnlyConfigStore.hpp"
#include "gtest.h"

class test_parseDirective : public ::testing::Test {
 protected:
  bool flg_thrown = false;
  ServerOnlyConfigStore store;
  std::list<std::string> settings;

  // 各ケース共通の前処理を書く
  virtual void SetUp() {}

  // 各ケース共通の後処理を書く
  virtual void TearDown() {}
};

TEST_F(test_parseDirective, root) {
  EXPECT_THROW(store.parseDirective("listen", settings), std::runtime_error);
  settings.push_back("localhost:80");
  EXPECT_TRUE(store.parseDirective("listen", settings));
  EXPECT_EQ(store.getListen().front().first, inet_addr("127.0.0.1"));
  EXPECT_EQ(store.getListen().front().second, htons(80));
}

TEST_F(test_parseDirective, server_name) {
  EXPECT_THROW(store.parseDirective("server_name", settings),
               std::runtime_error);
  settings.push_back("server");
  EXPECT_TRUE(store.parseDirective("server_name", settings));
  EXPECT_EQ(store.getServerName().front(), "server");
}
