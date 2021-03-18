/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parseAuthBasicUserFile.cc                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/18 14:44:11 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommonConfigStore.hpp"
#include "gtest.h"

class test_parseAuthBasicUserFile : public ::testing::Test {
 protected:
  bool flg_thrown = false;
  CommonConfigStore store;
  std::list<std::string> settings;

  // 各ケース共通の前処理を書く
  virtual void SetUp() {}

  // 各ケース共通の後処理を書く
  virtual void TearDown() {}
};

TEST_F(test_parseAuthBasicUserFile, normalOne) {
  settings.push_back(".htpasswd");
  EXPECT_NO_THROW(store.parseAuthBasicUserFile(settings));
  EXPECT_EQ(store.getAuthBasicUserFile(), settings);
}

TEST_F(test_parseAuthBasicUserFile, normalThree) {
  settings.push_back(".htpasswd");
  settings.push_back("/hoge/.htpasswd");
  settings.push_back("/fuga/.htpasswd");
  EXPECT_NO_THROW(store.parseAuthBasicUserFile(settings));
  EXPECT_EQ(store.getAuthBasicUserFile(), settings);
}

TEST_F(test_parseAuthBasicUserFile, normalOneTwice) {
  settings.push_back(".htpasswd");
  EXPECT_NO_THROW(store.parseAuthBasicUserFile(settings));
  settings.front() = "/hoge/.htpasswd";
  EXPECT_NO_THROW(store.parseAuthBasicUserFile(settings));
  std::list<std::string>::const_iterator itr =
      store.getAuthBasicUserFile().begin();
  EXPECT_EQ(*itr, ".htpasswd");
  EXPECT_EQ(*++itr, "/hoge/.htpasswd");
}

TEST_F(test_parseAuthBasicUserFile, normalOneTwice3times) {
  settings.push_back(".htpasswd");
  EXPECT_NO_THROW(store.parseAuthBasicUserFile(settings));
  settings.front() = "/hoge/.htpasswd";
  EXPECT_NO_THROW(store.parseAuthBasicUserFile(settings));
  settings.front() = "/fuga/.htpasswd";
  EXPECT_NO_THROW(store.parseAuthBasicUserFile(settings));
  std::list<std::string>::const_iterator itr =
      store.getAuthBasicUserFile().begin();
  EXPECT_EQ(*itr, ".htpasswd");
  EXPECT_EQ(*++itr, "/hoge/.htpasswd");
  EXPECT_EQ(*++itr, "/fuga/.htpasswd");
}

TEST_F(test_parseAuthBasicUserFile, settingInvalidNumber) {
  try {
    store.parseAuthBasicUserFile(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "auth_basic_user_file: invalid number of setting");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}
