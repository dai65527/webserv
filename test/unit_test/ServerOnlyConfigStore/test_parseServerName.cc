/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parseServerName.cc                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/19 12:38:15 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gtest.h"
#include "ServerOnlyConfigStore.hpp"

class test_parseServerName : public ::testing::Test {
 protected:
  bool flg_thrown = false;
  ServerOnlyConfigStore store;
  std::list<std::string> settings;

  // 各ケース共通の前処理を書く
  virtual void SetUp() {}

  // 各ケース共通の後処理を書く
  virtual void TearDown() {}
};

TEST_F(test_parseServerName, normalOne) {
  settings.push_back("server_name1");
  EXPECT_NO_THROW(store.parseServerName(settings));
  EXPECT_EQ(store.getServerName(), settings);
}

TEST_F(test_parseServerName, normalThree) {
  settings.push_back("server_name1");
  settings.push_back("server_name3");
  settings.push_back("server_name2");
  EXPECT_NO_THROW(store.parseServerName(settings));
  EXPECT_EQ(store.getServerName(), settings);
}

TEST_F(test_parseServerName, normalOneTwice) {
  settings.push_back("server_name1");
  EXPECT_NO_THROW(store.parseServerName(settings));
  settings.front() = "server_name2";
  EXPECT_NO_THROW(store.parseServerName(settings));
  std::list<std::string>::const_iterator itr = store.getServerName().begin();
  EXPECT_EQ(*itr, "server_name1");
  EXPECT_EQ(*++itr, "server_name2");
}

TEST_F(test_parseServerName, normalOneTwice3times) {
  settings.push_back("server_name1");
  EXPECT_NO_THROW(store.parseServerName(settings));
  settings.front() = "server_name3";
  EXPECT_NO_THROW(store.parseServerName(settings));
  settings.front() = "server_name2";
  EXPECT_NO_THROW(store.parseServerName(settings));
  std::list<std::string>::const_iterator itr = store.getServerName().begin();
  EXPECT_EQ(*itr, "server_name1");
  EXPECT_EQ(*++itr, "server_name3");
  EXPECT_EQ(*++itr, "server_name2");
}

TEST_F(test_parseServerName, settingInvalidNumber) {
  try {
    store.parseServerName(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "server_name: invalid number of setting");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}
