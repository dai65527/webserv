/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parseLanguage.cc                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/04/22 19:29:11 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "CommonConfigStore.hpp"
#include "gtest.h"

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

TEST_F(test_parseLanguage, normalOne_ja) {
  settings.push_back("ja");
  EXPECT_NO_THROW(store.parseLanguage(settings));
  std::list<std::string>::const_iterator itr_settings = settings.begin();
  std::list<std::string>::const_iterator itr_language =
      store.getLanguage().begin();
  while (itr_settings != settings.end()) {
    EXPECT_EQ(*itr_settings, *itr_language);
    ++itr_settings;
    ++itr_language;
  }
  EXPECT_EQ(itr_language, store.getLanguage().end());
}

TEST_F(test_parseLanguage, normalOne_ja_JP) {
  settings.push_back("ja-JP");
  EXPECT_NO_THROW(store.parseLanguage(settings));
  std::list<std::string>::const_iterator itr_settings = settings.begin();
  std::list<std::string>::const_iterator itr_language =
      store.getLanguage().begin();
  while (itr_settings != settings.end()) {
    EXPECT_EQ(*itr_settings, *itr_language);
    ++itr_settings;
    ++itr_language;
  }
  EXPECT_EQ(itr_language, store.getLanguage().end());
}

TEST_F(test_parseLanguage, normalFive) {
  settings.push_back("ja-JP");
  settings.push_back("en-GB");
  settings.push_back("en-US");
  settings.push_back("es");
  settings.push_back("zn");
  EXPECT_NO_THROW(store.parseLanguage(settings));
  std::list<std::string>::const_iterator itr_settings = settings.begin();
  std::list<std::string>::const_iterator itr_language =
      store.getLanguage().begin();
  while (itr_settings != settings.end()) {
    EXPECT_EQ(*itr_settings, *itr_language);
    ++itr_settings;
    ++itr_language;
  }
  EXPECT_EQ(itr_language, store.getLanguage().end());
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

// no need to specify format
/*
TEST_F(test_parseLanguage, duplicateSetting) {
  settings.push_back("ja");
  try {
    store.parseLanguage(settings);
    store.parseLanguage(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "language: directive duplicated");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseLanguage, invalidFormat1) {
  settings.push_back("ja");
  settings.push_back("j");
  settings.push_back("en");
  try {
    store.parseLanguage(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "language: invalid value \"j\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseLanguage, invalidFormat2) {
  settings.push_back("ja");
  settings.push_back("j-");
  settings.push_back("en");
  try {
    store.parseLanguage(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "language: invalid value \"j-\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseLanguage, invalidFormat3) {
  settings.push_back("ja");
  settings.push_back("ja-");
  settings.push_back("en");
  try {
    store.parseLanguage(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "language: invalid value \"ja-\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseLanguage, invalidFormat4) {
  settings.push_back("ja");
  settings.push_back("ja-");
  settings.push_back("en");
  try {
    store.parseLanguage(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "language: invalid value \"ja-\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseLanguage, invalidFormat5) {
  settings.push_back("ja");
  settings.push_back("ja-JPP");
  settings.push_back("en");
  try {
    store.parseLanguage(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "language: invalid value \"ja-JPP\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseLanguage, invalidFormat) {
  settings.push_back("ja");
  settings.push_back("JAPAN");
  settings.push_back("en");
  try {
    store.parseLanguage(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "language: invalid value \"JAPAN\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}
*/
