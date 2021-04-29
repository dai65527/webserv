/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parseRetryAfter.cc                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/04/28 17:11:31 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "MainOnlyConfigStore.hpp"
#include "gtest.h"

class test_parseRetryAfter : public ::testing::Test {
 protected:
  bool flg_thrown = false;
  MainOnlyConfigStore store;
  std::list<std::string> settings;

  // 各ケース共通の前処理を書く
  virtual void SetUp() {}

  // 各ケース共通の後処理を書く
  virtual void TearDown() {}
};

TEST_F(test_parseRetryAfter, defaultVal) {
  EXPECT_EQ(store.getRetryAfter(), 120);
}

TEST_F(test_parseRetryAfter, normal10) {
  settings.push_back("10");
  EXPECT_NO_THROW(store.parseRetryAfter(settings));
  EXPECT_EQ(store.getRetryAfter(), 10);
}

TEST_F(test_parseRetryAfter, normal42) {
  settings.push_back("42");
  EXPECT_NO_THROW(store.parseRetryAfter(settings));
  EXPECT_EQ(store.getRetryAfter(), 42);
}

TEST_F(test_parseRetryAfter, normal4200) {
  settings.push_back("4200");
  EXPECT_NO_THROW(store.parseRetryAfter(settings));
  EXPECT_EQ(store.getRetryAfter(), 4200);
}

TEST_F(test_parseRetryAfter, normal2147483647) {
  settings.push_back("2147483647");
  EXPECT_NO_THROW(store.parseRetryAfter(settings));
  EXPECT_EQ(store.getRetryAfter(), 2147483647);
}

TEST_F(test_parseRetryAfter, emptySetting) {
  try {
    store.parseRetryAfter(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "retry_after: invalid number of setting");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseRetryAfter, duplicateSetting) {
  settings.push_back("42");
  try {
    store.parseRetryAfter(settings);
    store.parseRetryAfter(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "retry_after: directive duplicated");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseRetryAfter, settingInvalidNumberOfSetting) {
  settings.push_back("42");
  settings.push_back("42");
  try {
    store.parseRetryAfter(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "retry_after: invalid number of setting");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseRetryAfter, errorInvalidValue0) {
  settings.push_back("-42");
  try {
    store.parseRetryAfter(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "retry_after: invalid value \"-42\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseRetryAfter, errorInvalidValue2147483648) {
  settings.push_back("2147483648");
  try {
    store.parseRetryAfter(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "retry_after: invalid value \"2147483648\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseRetryAfter, errorInvalidValue9999999999) {
  settings.push_back("9999999999");
  try {
    store.parseRetryAfter(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "retry_after: invalid value \"9999999999\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseRetryAfter, errorInvalidValue99999999999999999) {
  settings.push_back("99999999999999999");
  try {
    store.parseRetryAfter(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "retry_after: invalid value \"99999999999999999\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseRetryAfter, errorInvalidValueNeg42) {
  settings.push_back("-42");
  try {
    store.parseRetryAfter(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "retry_after: invalid value \"-42\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseRetryAfter, errorInvalidValueHoge) {
  settings.push_back("hoge");
  try {
    store.parseRetryAfter(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "retry_after: invalid value \"hoge\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}
