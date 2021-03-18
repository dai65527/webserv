/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parseLanguage.cc                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/18 13:38:11 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gtest.h"
#include "CommonConfigStore.hpp"

class test_parseLanguage : public ::testing::Test {
 protected:
  bool flg_thrown = false;
  CommonConfigStore store;
  std::list<std::string> settings;

  // 各ケース共通の前処理を書く
  virtual void SetUp() {}

  // 各ケース共通の後処理を書く
  virtual void TearDown() {}
};

TEST_F(test_parseLanguage, normal) {
  settings.push_back("utf-8");
  EXPECT_NO_THROW(store.parseLanguage(settings));
  EXPECT_EQ(store.getLanguage(), settings.front());
}

TEST_F(test_parseLanguage, emptySetting) {
  try {
    store.parseLanguage(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "language: invalid number of setting");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseLanguage, duplicateSetting) {
  settings.push_back("utf-8");
  try {
    store.parseLanguage(settings);
    store.parseLanguage(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "language: directive duplicated");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseLanguage, settingInvalidNumber) {
  settings.push_back("utf-8");
  settings.push_back("utf-8");
  try {
    store.parseLanguage(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "language: invalid number of setting");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}
