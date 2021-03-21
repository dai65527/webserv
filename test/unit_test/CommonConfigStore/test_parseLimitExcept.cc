/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parseLimitExcept.cc                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/21 09:16:38 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "CommonConfigStore.hpp"
#include "gtest.h"

class test_parseLimitExcept : public ::testing::Test {
 protected:
  bool flg_thrown = false;
  CommonConfigStore store;
  std::list<std::string> settings;

  // 各ケース共通の前処理を書く
  virtual void SetUp() {}

  // 各ケース共通の後処理を書く
  virtual void TearDown() {}
};

TEST_F(test_parseLimitExcept, defaultVal) {
  EXPECT_EQ(store.getLimitExcept(), HTTP_GET | HTTP_HEAD);
}

TEST_F(test_parseLimitExcept, onlyGET) {
  settings.push_back("GET");
  EXPECT_NO_THROW(store.parseLimitExcept(settings));
  EXPECT_EQ(store.getLimitExcept(), HTTP_GET | HTTP_HEAD);
}

TEST_F(test_parseLimitExcept, onlyHEAD) {
  settings.push_back("HEAD");
  EXPECT_NO_THROW(store.parseLimitExcept(settings));
  EXPECT_EQ(store.getLimitExcept(), HTTP_HEAD);
}

TEST_F(test_parseLimitExcept, onlyPUT) {
  settings.push_back("PUT");
  EXPECT_NO_THROW(store.parseLimitExcept(settings));
  EXPECT_EQ(store.getLimitExcept(), HTTP_PUT);
}

TEST_F(test_parseLimitExcept, onlyPOST) {
  settings.push_back("PUT");
  EXPECT_NO_THROW(store.parseLimitExcept(settings));
  EXPECT_EQ(store.getLimitExcept(), HTTP_PUT);
}

TEST_F(test_parseLimitExcept, onlyDELETE) {
  settings.push_back("DELETE");
  EXPECT_NO_THROW(store.parseLimitExcept(settings));
  EXPECT_EQ(store.getLimitExcept(), HTTP_DELETE);
}

TEST_F(test_parseLimitExcept, onlyOPTIONS) {
  settings.push_back("OPTIONS");
  EXPECT_NO_THROW(store.parseLimitExcept(settings));
  EXPECT_EQ(store.getLimitExcept(), HTTP_OPTIONS);
}

TEST_F(test_parseLimitExcept, onlyTRACE) {
  settings.push_back("TRACE");
  EXPECT_NO_THROW(store.parseLimitExcept(settings));
  EXPECT_EQ(store.getLimitExcept(), HTTP_TRACE);
}

TEST_F(test_parseLimitExcept, GET_HEAD) {
  settings.push_back("GET");
  settings.push_back("HEAD");
  EXPECT_NO_THROW(store.parseLimitExcept(settings));
  EXPECT_EQ(store.getLimitExcept(), HTTP_GET | HTTP_HEAD);
}

TEST_F(test_parseLimitExcept, GET_PUT) {
  settings.push_back("GET");
  settings.push_back("PUT");
  EXPECT_NO_THROW(store.parseLimitExcept(settings));
  EXPECT_EQ(store.getLimitExcept(), HTTP_GET | HTTP_HEAD | HTTP_PUT);
}

TEST_F(test_parseLimitExcept, GET_POST) {
  settings.push_back("GET");
  settings.push_back("POST");
  EXPECT_NO_THROW(store.parseLimitExcept(settings));
  EXPECT_EQ(store.getLimitExcept(), HTTP_GET | HTTP_HEAD | HTTP_POST);
}

TEST_F(test_parseLimitExcept, GET_DELETE) {
  settings.push_back("GET");
  settings.push_back("DELETE");
  EXPECT_NO_THROW(store.parseLimitExcept(settings));
  EXPECT_EQ(store.getLimitExcept(), HTTP_GET | HTTP_HEAD | HTTP_DELETE);
}

TEST_F(test_parseLimitExcept, GET_OPTIONS) {
  settings.push_back("GET");
  settings.push_back("OPTIONS");
  EXPECT_NO_THROW(store.parseLimitExcept(settings));
  EXPECT_EQ(store.getLimitExcept(), HTTP_GET | HTTP_HEAD | HTTP_OPTIONS);
}

TEST_F(test_parseLimitExcept, GET_TRACE) {
  settings.push_back("GET");
  settings.push_back("TRACE");
  EXPECT_NO_THROW(store.parseLimitExcept(settings));
  EXPECT_EQ(store.getLimitExcept(), HTTP_GET | HTTP_HEAD | HTTP_TRACE);
}

TEST_F(test_parseLimitExcept, GET_PUT_POST_DELETE) {
  settings.push_back("GET");
  settings.push_back("PUT");
  settings.push_back("POST");
  settings.push_back("DELETE");
  EXPECT_NO_THROW(store.parseLimitExcept(settings));
  EXPECT_EQ(store.getLimitExcept(),
            HTTP_GET | HTTP_HEAD | HTTP_PUT | HTTP_POST | HTTP_DELETE);
}

TEST_F(test_parseLimitExcept, GET_HEAD_PUT_POST_DELETE) {
  settings.push_back("GET");
  settings.push_back("HEAD");
  settings.push_back("PUT");
  settings.push_back("POST");
  settings.push_back("DELETE");
  EXPECT_NO_THROW(store.parseLimitExcept(settings));
  EXPECT_EQ(store.getLimitExcept(),
            HTTP_GET | HTTP_HEAD | HTTP_PUT | HTTP_POST | HTTP_DELETE);
}

// Don't support CONNECT method
TEST_F(test_parseLimitExcept, errorOnlyCONNECT) {
  settings.push_back("CONNECT");
  try {
    store.parseLimitExcept(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "limit_except: CONNECT method not supported");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseLimitExcept, dupulicateSetting) {
  settings.push_back("GET");
  settings.push_back("HEAD");
  EXPECT_NO_THROW(store.parseLimitExcept(settings));
  settings.clear();
  settings.push_back("POST");
  try {
    store.parseLimitExcept(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "limit_except: directive duplicated");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseLimitExcept, errorInvalidValueHoge) {
  settings.push_back("HOGE");
  try {
    store.parseLimitExcept(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "limit_except: invalid value \"HOGE\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseLimitExcept, errorInvalidValueGET_Hoge) {
  settings.push_back("GET");
  settings.push_back("HOGE");
  try {
    store.parseLimitExcept(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "limit_except: invalid value \"HOGE\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseLimitExcept, errorInvalidValueHOGE_GET) {
  settings.push_back("HOGE");
  settings.push_back("GET");
  try {
    store.parseLimitExcept(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "limit_except: invalid value \"HOGE\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseLimitExcept, errorEmptySetting) {
  try {
    store.parseLimitExcept(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "limit_except: invalid number of setting");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}
