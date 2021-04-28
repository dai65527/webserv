/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parseMaxSessions.cc                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/04/28 16:07:56 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "MainOnlyConfigStore.hpp"
#include "gtest.h"

class test_parseMaxSessions : public ::testing::Test {
 protected:
  bool flg_thrown = false;
  MainOnlyConfigStore store;
  std::list<std::string> settings;

  // 各ケース共通の前処理を書く
  virtual void SetUp() {}

  // 各ケース共通の後処理を書く
  virtual void TearDown() {}
};

TEST_F(test_parseMaxSessions, defaultVal) {
  EXPECT_EQ(store.getMaxSessions(), 1000);
}

TEST_F(test_parseMaxSessions, normal10) {
  settings.push_back("10");
  EXPECT_NO_THROW(store.parseMaxSessions(settings));
  EXPECT_EQ(store.getMaxSessions(), 10);
}

TEST_F(test_parseMaxSessions, normal42) {
  settings.push_back("42");
  EXPECT_NO_THROW(store.parseMaxSessions(settings));
  EXPECT_EQ(store.getMaxSessions(), 42);
}

TEST_F(test_parseMaxSessions, normal4200) {
  settings.push_back("4200");
  EXPECT_NO_THROW(store.parseMaxSessions(settings));
  EXPECT_EQ(store.getMaxSessions(), 4200);
}

TEST_F(test_parseMaxSessions, normal2147483647) {
  settings.push_back("2147483647");
  EXPECT_NO_THROW(store.parseMaxSessions(settings));
  EXPECT_EQ(store.getMaxSessions(), 2147483647);
}

TEST_F(test_parseMaxSessions, emptySetting) {
  try {
    store.parseMaxSessions(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "max_sessions: invalid number of setting");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseMaxSessions, duplicateSetting) {
  settings.push_back("42");
  try {
    store.parseMaxSessions(settings);
    store.parseMaxSessions(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "max_sessions: directive duplicated");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseMaxSessions, settingInvalidNumberOfSetting) {
  settings.push_back("42");
  settings.push_back("42");
  try {
    store.parseMaxSessions(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "max_sessions: invalid number of setting");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseMaxSessions, errorInvalidValue0) {
  settings.push_back("-42");
  try {
    store.parseMaxSessions(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "max_sessions: invalid value \"-42\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseMaxSessions, errorInvalidValue2147483648) {
  settings.push_back("2147483648");
  try {
    store.parseMaxSessions(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "max_sessions: invalid value \"2147483648\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseMaxSessions, errorInvalidValue9999999999) {
  settings.push_back("9999999999");
  try {
    store.parseMaxSessions(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "max_sessions: invalid value \"9999999999\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseMaxSessions, errorInvalidValue99999999999999999) {
  settings.push_back("99999999999999999");
  try {
    store.parseMaxSessions(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "max_sessions: invalid value \"99999999999999999\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseMaxSessions, errorInvalidValueNeg42) {
  settings.push_back("-42");
  try {
    store.parseMaxSessions(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "max_sessions: invalid value \"-42\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseMaxSessions, errorInvalidValueHoge) {
  settings.push_back("hoge");
  try {
    store.parseMaxSessions(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "max_sessions: invalid value \"hoge\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}
