/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parseBaseAuth.cc                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/18 14:08:43 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gtest.h"
#include "CommonConfigStore.hpp"

class test_parseBaseAuth : public ::testing::Test {
 protected:
  bool flg_thrown = false;
  CommonConfigStore store;
  std::list<std::string> settings;

  // 各ケース共通の前処理を書く
  virtual void SetUp() {}

  // 各ケース共通の後処理を書く
  virtual void TearDown() {}
};

TEST_F(test_parseBaseAuth, normalOn) {
  settings.push_back("need authentication");
  EXPECT_NO_THROW(store.parseBaseAuth(settings));
  EXPECT_EQ(store.getBaseAuth(), settings.front());
}

TEST_F(test_parseBaseAuth, normalOff) {
  settings.push_back("off");
  EXPECT_NO_THROW(store.parseBaseAuth(settings));
  EXPECT_EQ(store.getBaseAuth(), settings.front());
  EXPECT_STREQ(store.getBaseAuth().c_str(), "off");
}

TEST_F(test_parseBaseAuth, defaultOff) {
  EXPECT_TRUE(store.getBaseAuth().empty()); // base auth_off
}

TEST_F(test_parseBaseAuth, emptySetting) {
  try {
    store.parseBaseAuth(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "base_auth: invalid number of setting");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseBaseAuth, duplicateSetting) {
  settings.push_back("need authentication");
  try {
    store.parseBaseAuth(settings);
    store.parseBaseAuth(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "base_auth: directive duplicated");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseBaseAuth, settingInvalidNumber) {
  settings.push_back("need authentication");
  settings.push_back("need authentication");
  try {
    store.parseBaseAuth(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "base_auth: invalid number of setting");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}
