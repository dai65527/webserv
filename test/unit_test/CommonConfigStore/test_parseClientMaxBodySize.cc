/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parseClientMaxBodySize.cc                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/18 18:39:30 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gtest.h"
#include "CommonConfigStore.hpp"

class test_parseClientMaxBodySize : public ::testing::Test {
 protected:
  bool flg_thrown = false;
  CommonConfigStore store;
  std::list<std::string> settings;

  // 各ケース共通の前処理を書く
  virtual void SetUp() {}

  // 各ケース共通の後処理を書く
  virtual void TearDown() {}
};

TEST_F(test_parseClientMaxBodySize, defaultVal) {
  EXPECT_EQ(store.getClientMaxBodySize(), 1000000);
}

TEST_F(test_parseClientMaxBodySize, normal0) {
  settings.push_back("0");
  EXPECT_NO_THROW(store.parseClientMaxBodySize(settings));
  EXPECT_EQ(store.getClientMaxBodySize(), 0);
}

TEST_F(test_parseClientMaxBodySize, normal10) {
  settings.push_back("10");
  EXPECT_NO_THROW(store.parseClientMaxBodySize(settings));
  EXPECT_EQ(store.getClientMaxBodySize(), 10);
}

TEST_F(test_parseClientMaxBodySize, normal42) {
  settings.push_back("42");
  EXPECT_NO_THROW(store.parseClientMaxBodySize(settings));
  EXPECT_EQ(store.getClientMaxBodySize(), 42);
}

TEST_F(test_parseClientMaxBodySize, normal4200) {
  settings.push_back("4200");
  EXPECT_NO_THROW(store.parseClientMaxBodySize(settings));
  EXPECT_EQ(store.getClientMaxBodySize(), 4200);
}

TEST_F(test_parseClientMaxBodySize, normal18446744073709551615) {
  settings.push_back("18446744073709551615");
  EXPECT_NO_THROW(store.parseClientMaxBodySize(settings));
  EXPECT_EQ(store.getClientMaxBodySize(), 18446744073709551615UL);
}

TEST_F(test_parseClientMaxBodySize, normal0k) {
  settings.push_back("0k");
  EXPECT_NO_THROW(store.parseClientMaxBodySize(settings));
  EXPECT_EQ(store.getClientMaxBodySize(), 0);
}

TEST_F(test_parseClientMaxBodySize, normal0K) {
  settings.push_back("0K");
  EXPECT_NO_THROW(store.parseClientMaxBodySize(settings));
  EXPECT_EQ(store.getClientMaxBodySize(), 0);
}

TEST_F(test_parseClientMaxBodySize, normal1k) {
  settings.push_back("1k");
  EXPECT_NO_THROW(store.parseClientMaxBodySize(settings));
  EXPECT_EQ(store.getClientMaxBodySize(), 1000);
}

TEST_F(test_parseClientMaxBodySize, normal1K) {
  settings.push_back("1K");
  EXPECT_NO_THROW(store.parseClientMaxBodySize(settings));
  EXPECT_EQ(store.getClientMaxBodySize(), 1000);
}

TEST_F(test_parseClientMaxBodySize, normal42k) {
  settings.push_back("42k");
  EXPECT_NO_THROW(store.parseClientMaxBodySize(settings));
  EXPECT_EQ(store.getClientMaxBodySize(), 42000);
}

TEST_F(test_parseClientMaxBodySize, normal42K) {
  settings.push_back("42K");
  EXPECT_NO_THROW(store.parseClientMaxBodySize(settings));
  EXPECT_EQ(store.getClientMaxBodySize(), 42000);
}

TEST_F(test_parseClientMaxBodySize, normal42000k) {
  settings.push_back("42000k");
  EXPECT_NO_THROW(store.parseClientMaxBodySize(settings));
  EXPECT_EQ(store.getClientMaxBodySize(), 42000000);
}

TEST_F(test_parseClientMaxBodySize, normal42000K) {
  settings.push_back("42000K");
  EXPECT_NO_THROW(store.parseClientMaxBodySize(settings));
  EXPECT_EQ(store.getClientMaxBodySize(), 42000000);
}

TEST_F(test_parseClientMaxBodySize, normal0m) {
  settings.push_back("0m");
  EXPECT_NO_THROW(store.parseClientMaxBodySize(settings));
  EXPECT_EQ(store.getClientMaxBodySize(), 0);
}

TEST_F(test_parseClientMaxBodySize, normal0M) {
  settings.push_back("0M");
  EXPECT_NO_THROW(store.parseClientMaxBodySize(settings));
  EXPECT_EQ(store.getClientMaxBodySize(), 0);
}

TEST_F(test_parseClientMaxBodySize, normal1m) {
  settings.push_back("1m");
  EXPECT_NO_THROW(store.parseClientMaxBodySize(settings));
  EXPECT_EQ(store.getClientMaxBodySize(), 1000000);
}

TEST_F(test_parseClientMaxBodySize, normal1M) {
  settings.push_back("1M");
  EXPECT_NO_THROW(store.parseClientMaxBodySize(settings));
  EXPECT_EQ(store.getClientMaxBodySize(), 1000000);
}

TEST_F(test_parseClientMaxBodySize, normal42m) {
  settings.push_back("42m");
  EXPECT_NO_THROW(store.parseClientMaxBodySize(settings));
  EXPECT_EQ(store.getClientMaxBodySize(), 42000000UL);
}

TEST_F(test_parseClientMaxBodySize, normal42M) {
  settings.push_back("42M");
  EXPECT_NO_THROW(store.parseClientMaxBodySize(settings));
  EXPECT_EQ(store.getClientMaxBodySize(), 42000000UL);
}

TEST_F(test_parseClientMaxBodySize, normal42000m) {
  settings.push_back("42000m");
  EXPECT_NO_THROW(store.parseClientMaxBodySize(settings));
  EXPECT_EQ(store.getClientMaxBodySize(), 42000000000UL);
}

TEST_F(test_parseClientMaxBodySize, normal42000M) {
  settings.push_back("42000M");
  EXPECT_NO_THROW(store.parseClientMaxBodySize(settings));
  EXPECT_EQ(store.getClientMaxBodySize(), 42000000000UL);
}

TEST_F(test_parseClientMaxBodySize, normal0g) {
  settings.push_back("0g");
  EXPECT_NO_THROW(store.parseClientMaxBodySize(settings));
  EXPECT_EQ(store.getClientMaxBodySize(), 0);
}

TEST_F(test_parseClientMaxBodySize, normal0G) {
  settings.push_back("0G");
  EXPECT_NO_THROW(store.parseClientMaxBodySize(settings));
  EXPECT_EQ(store.getClientMaxBodySize(), 0);
}

TEST_F(test_parseClientMaxBodySize, normal1g) {
  settings.push_back("1g");
  EXPECT_NO_THROW(store.parseClientMaxBodySize(settings));
  EXPECT_EQ(store.getClientMaxBodySize(), 1000000000);
}

TEST_F(test_parseClientMaxBodySize, normal1G) {
  settings.push_back("1G");
  EXPECT_NO_THROW(store.parseClientMaxBodySize(settings));
  EXPECT_EQ(store.getClientMaxBodySize(), 1000000000);
}

TEST_F(test_parseClientMaxBodySize, normal42g) {
  settings.push_back("42g");
  EXPECT_NO_THROW(store.parseClientMaxBodySize(settings));
  EXPECT_EQ(store.getClientMaxBodySize(), 42000000000UL);
}

TEST_F(test_parseClientMaxBodySize, normal42G) {
  settings.push_back("42G");
  EXPECT_NO_THROW(store.parseClientMaxBodySize(settings));
  EXPECT_EQ(store.getClientMaxBodySize(), 42000000000UL);
}

TEST_F(test_parseClientMaxBodySize, normal42000g) {
  settings.push_back("42000000m");
  EXPECT_NO_THROW(store.parseClientMaxBodySize(settings));
  EXPECT_EQ(store.getClientMaxBodySize(), 42000000000000UL);
}

TEST_F(test_parseClientMaxBodySize, normal42000G) {
  settings.push_back("42000000M");
  EXPECT_NO_THROW(store.parseClientMaxBodySize(settings));
  EXPECT_EQ(store.getClientMaxBodySize(), 42000000000000UL);
}

TEST_F(test_parseClientMaxBodySize, emptySetting) {
  try {
    store.parseClientMaxBodySize(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "client_max_body_size: invalid number of setting");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseClientMaxBodySize, duplicateSetting) {
  settings.push_back("42m");
  try {
    store.parseClientMaxBodySize(settings);
    store.parseClientMaxBodySize(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "client_max_body_size: directive duplicated");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseClientMaxBodySize, settingInvalidNumberOfSetting) {
  settings.push_back("42k");
  settings.push_back("42m");
  try {
    store.parseClientMaxBodySize(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "client_max_body_size: invalid number of setting");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseClientMaxBodySize, errorInvalidValueOnlyK) {
  settings.push_back("K");
  try {
    store.parseClientMaxBodySize(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "client_max_body_size: invalid value \"K\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseClientMaxBodySize, errorInvalidValueOnlyM) {
  settings.push_back("M");
  try {
    store.parseClientMaxBodySize(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "client_max_body_size: invalid value \"M\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseClientMaxBodySize, errorInvalidValueOnlyG) {
  settings.push_back("G");
  try {
    store.parseClientMaxBodySize(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "client_max_body_size: invalid value \"G\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseClientMaxBodySize, errorInvalidValueNeg42) {
  settings.push_back("-42");
  try {
    store.parseClientMaxBodySize(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "client_max_body_size: invalid value \"-42\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseClientMaxBodySize, errorInvalidValueNeg42k) {
  settings.push_back("-42k");
  try {
    store.parseClientMaxBodySize(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "client_max_body_size: invalid value \"-42k\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseClientMaxBodySize, errorInvalidValueNeg42m) {
  settings.push_back("-42m");
  try {
    store.parseClientMaxBodySize(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "client_max_body_size: invalid value \"-42m\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseClientMaxBodySize, errorInvalidValueNeg42G) {
  settings.push_back("-42G");
  try {
    store.parseClientMaxBodySize(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "client_max_body_size: invalid value \"-42G\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseClientMaxBodySize, errorInvalidValueHoge) {
  settings.push_back("hoge");
  try {
    store.parseClientMaxBodySize(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "client_max_body_size: invalid value \"hoge\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}
