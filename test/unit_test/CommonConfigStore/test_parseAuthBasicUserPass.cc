/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parseAuthBasicUserPass.cc                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/04/30 15:05:05 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "CommonConfigStore.hpp"
#include "gtest.h"

class test_parseAuthBasicUserPass : public ::testing::Test {
 protected:
  bool flg_thrown = false;
  CommonConfigStore store;
  std::list<std::string> settings;

  // 各ケース共通の前処理を書く
  virtual void SetUp() {}

  // 各ケース共通の後処理を書く
  virtual void TearDown() {}
};

TEST_F(test_parseAuthBasicUserPass, normalOne) {
  settings.push_back("user:pass");
  EXPECT_NO_THROW(store.parseAuthBasicUserPass(settings));
  EXPECT_EQ(store.getAuthBasicUserPass(), settings);
}

TEST_F(test_parseAuthBasicUserPass, normalThree) {
  settings.push_back("user:pass");
  settings.push_back("user1:pass1");
  settings.push_back("hoge:fuga");
  EXPECT_NO_THROW(store.parseAuthBasicUserPass(settings));
  EXPECT_EQ(store.getAuthBasicUserPass(), settings);
}

TEST_F(test_parseAuthBasicUserPass, normalOneTwice) {
  settings.push_back("user:pass");
  EXPECT_NO_THROW(store.parseAuthBasicUserPass(settings));
  settings.front() = "user1:pass1";
  EXPECT_NO_THROW(store.parseAuthBasicUserPass(settings));
  std::list<std::string>::const_iterator itr =
      store.getAuthBasicUserPass().begin();
  EXPECT_EQ(*itr, "user:pass");
  EXPECT_EQ(*++itr, "user1:pass1");
}

TEST_F(test_parseAuthBasicUserPass, normalOneTwice3times) {
  settings.push_back("user:pass");
  EXPECT_NO_THROW(store.parseAuthBasicUserPass(settings));
  settings.front() = "user1:pass1";
  EXPECT_NO_THROW(store.parseAuthBasicUserPass(settings));
  settings.front() = "user2:";
  EXPECT_NO_THROW(store.parseAuthBasicUserPass(settings));
  std::list<std::string>::const_iterator itr =
      store.getAuthBasicUserPass().begin();
  EXPECT_EQ(*itr, "user:pass");
  EXPECT_EQ(*++itr, "user1:pass1");
  EXPECT_EQ(*++itr, "user2:");
}

TEST_F(test_parseAuthBasicUserPass, settingInvalidArgNoColon) {
  try {
    settings.push_back("user");
    store.parseAuthBasicUserPass(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "auth_basic_user_pass: invalid value \"user\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseAuthBasicUserPass, settingInvalidArgDupColon) {
  try {
    settings.push_back("user:password:hoge");
    store.parseAuthBasicUserPass(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(),
                 "auth_basic_user_pass: invalid value \"user:password:hoge\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseAuthBasicUserPass, settingInvalidArgNoUserName) {
  try {
    settings.push_back(":password");
    store.parseAuthBasicUserPass(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "auth_basic_user_pass: invalid value \":password\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseAuthBasicUserPass, settingInvalidNumber) {
  try {
    store.parseAuthBasicUserPass(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "auth_basic_user_pass: invalid number of setting");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}
