/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parseUploadStore.cc                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/04/28 16:05:35 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "ServerLocationConfigStore.hpp"
#include "gtest.h"

class test_parseUploadStore : public ::testing::Test {
 protected:
  bool flg_thrown = false;
  ServerLocationConfigStore store;
  std::list<std::string> settings;

  // 各ケース共通の前処理を書く
  virtual void SetUp() {}

  // 各ケース共通の後処理を書く
  virtual void TearDown() {}
};

TEST_F(test_parseUploadStore, normal) {
  settings.push_back("/pass/to/route");
  EXPECT_NO_THROW(store.parseUploadStore(settings));
  EXPECT_EQ(store.getUploadStore(), "/pass/to/route");
}

TEST_F(test_parseUploadStore, emptySetting) {
  try {
    store.parseUploadStore(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "upload_store: invalid number of setting");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseUploadStore, duplicateSetting) {
  settings.push_back("/upload_store/");
  try {
    store.parseUploadStore(settings);
    store.parseUploadStore(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "upload_store: directive duplicated");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseUploadStore, settingInvalidNumber) {
  settings.push_back("/upload_store");
  settings.push_back("/upload_store2");
  try {
    store.parseUploadStore(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "upload_store: invalid number of setting");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}
