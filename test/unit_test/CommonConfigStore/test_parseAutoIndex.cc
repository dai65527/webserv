/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parseAutoIndex.cc                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/18 12:15:44 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gtest.h"
#include "CommonConfigStore.hpp"

class test_parseAutoIndex : public ::testing::Test {
 protected:
  bool flg_thrown = false;
  CommonConfigStore store;
  std::list<std::string> settings;

  // 各ケース共通の前処理を書く
  virtual void SetUp() {}

  // 各ケース共通の後処理を書く
  virtual void TearDown() {}
};

TEST_F(test_parseAutoIndex, normalOn) {
  settings.push_back("on");
  EXPECT_NO_THROW(store.parseAutoIndex(settings));
  EXPECT_EQ(store.getAutoIndex(), true);
}

TEST_F(test_parseAutoIndex, normalOff) {
  settings.push_back("off");
  EXPECT_NO_THROW(store.parseAutoIndex(settings));
  EXPECT_EQ(store.getAutoIndex(), false);
}

TEST_F(test_parseAutoIndex, errorEmptySetting) {
  try {
    store.parseAutoIndex(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "autoindex: invalid number of setting");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseAutoIndex, errorSettingInvalidNumber) {
  settings.push_back("on");
  settings.push_back("on");
  try {
    store.parseAutoIndex(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "autoindex: invalid number of setting");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseAutoIndex, errorDuplicateSetting) {
  settings.push_back("on");
  try {
    store.parseAutoIndex(settings);
    store.parseAutoIndex(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "autoindex: directive duplicated");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseAutoIndex, errorInvalidSetting) {
  settings.push_back("hoge");
  try {
    store.parseAutoIndex(settings);
    store.parseAutoIndex(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "autoindex: invalid value \"hoge\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}
