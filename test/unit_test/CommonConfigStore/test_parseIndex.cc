/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parseIndex.cc                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/17 22:08:21 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gtest.h"
#include "CommonConfigStore.hpp"

class test_parseIndex : public ::testing::Test {
 protected:
  bool flg_thrown = false;
  CommonConfigStore store;
  std::list<std::string> settings;

  // 各ケース共通の前処理を書く
  virtual void SetUp() {}

  // 各ケース共通の後処理を書く
  virtual void TearDown() {}
};

TEST_F(test_parseIndex, normalOne) {
  settings.push_back("index.html");
  EXPECT_NO_THROW(store.parseIndex(settings));
  EXPECT_EQ(store.getIndex(), settings);
}

TEST_F(test_parseIndex, normalThree) {
  settings.push_back("index.html");
  settings.push_back("index.htm");
  settings.push_back("index.php");
  EXPECT_NO_THROW(store.parseIndex(settings));
  EXPECT_EQ(store.getIndex(), settings);
}

TEST_F(test_parseIndex, normalOneTwice) {
  settings.push_back("index.html");
  EXPECT_NO_THROW(store.parseIndex(settings));
  settings.front() = "index.php";
  EXPECT_NO_THROW(store.parseIndex(settings));
  std::list<std::string>::const_iterator itr = store.getIndex().begin();
  EXPECT_EQ(*itr, "index.html");
  EXPECT_EQ(*++itr, "index.php");
}

TEST_F(test_parseIndex, normalOneTwice3times) {
  settings.push_back("index.html");
  EXPECT_NO_THROW(store.parseIndex(settings));
  settings.front() = "index.htm";
  EXPECT_NO_THROW(store.parseIndex(settings));
  settings.front() = "index.php";
  EXPECT_NO_THROW(store.parseIndex(settings));
  std::list<std::string>::const_iterator itr = store.getIndex().begin();
  EXPECT_EQ(*itr, "index.html");
  EXPECT_EQ(*++itr, "index.htm");
  EXPECT_EQ(*++itr, "index.php");
}

TEST_F(test_parseIndex, settingInvalidNumber) {
  try {
    store.parseIndex(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "index: invalid number of setting");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}
