/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parseRoot.cc                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/17 22:07:35 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gtest.h"
#include "CommonConfigStore.hpp"

class test_parseRoot : public ::testing::Test {
 protected:
  bool flg_thrown = false;
  CommonConfigStore store;
  std::list<std::string> settings;

  // 各ケース共通の前処理を書く
  virtual void SetUp() {}

  // 各ケース共通の後処理を書く
  virtual void TearDown() {}
};

TEST_F(test_parseRoot, normal) {
  settings.push_back("/root/");
  EXPECT_NO_THROW(store.parseRoot(settings));
  EXPECT_EQ(store.getRoot(), settings.front());
}

TEST_F(test_parseRoot, emptySetting) {
  try {
    store.parseRoot(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "root: invalid number of setting");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseRoot, duplicateSetting) {
  settings.push_back("/root/");
  try {
    store.parseRoot(settings);
    store.parseRoot(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "root: directive duplicated");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseRoot, settingInvalidNumber) {
  settings.push_back("/root");
  settings.push_back("/root2");
  try {
    store.parseRoot(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "root: invalid number of setting");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}
