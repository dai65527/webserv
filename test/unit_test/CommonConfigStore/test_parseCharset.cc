/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parseCharset.cc                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/18 13:28:16 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gtest.h"
#include "CommonConfigStore.hpp"

class test_parseCharset : public ::testing::Test {
 protected:
  bool flg_thrown = false;
  CommonConfigStore store;
  std::list<std::string> settings;

  // 各ケース共通の前処理を書く
  virtual void SetUp() {}

  // 各ケース共通の後処理を書く
  virtual void TearDown() {}
};

TEST_F(test_parseCharset, normal) {
  settings.push_back("utf-8");
  EXPECT_NO_THROW(store.parseCharset(settings));
  EXPECT_EQ(store.getCharset(), settings.front());
}

TEST_F(test_parseCharset, emptySetting) {
  try {
    store.parseCharset(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "charset: invalid number of setting");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseCharset, duplicateSetting) {
  settings.push_back("utf-8");
  try {
    store.parseCharset(settings);
    store.parseCharset(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "charset: directive duplicated");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseCharset, settingInvalidNumber) {
  settings.push_back("utf-8");
  settings.push_back("utf-8");
  try {
    store.parseCharset(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "charset: invalid number of setting");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}
