/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parseUploadPass.cc                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/19 11:46:22 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerLocationConfigStore.hpp"
#include "gtest.h"

class test_parseUploadPass : public ::testing::Test {
 protected:
  bool flg_thrown = false;
  ServerLocationConfigStore store;
  std::list<std::string> settings;

  // 各ケース共通の前処理を書く
  virtual void SetUp() {}

  // 各ケース共通の後処理を書く
  virtual void TearDown() {}
};

TEST_F(test_parseUploadPass, normal) {
  settings.push_back("/pass/to/route");
  EXPECT_NO_THROW(store.parseUploadPass(settings));
  EXPECT_EQ(store.getUploadPass(), "/pass/to/route");
}

TEST_F(test_parseUploadPass, emptySetting) {
  try {
    store.parseUploadPass(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "upload_pass: invalid number of setting");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseUploadPass, duplicateSetting) {
  settings.push_back("/upload_pass/");
  try {
    store.parseUploadPass(settings);
    store.parseUploadPass(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "upload_pass: directive duplicated");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseUploadPass, settingInvalidNumber) {
  settings.push_back("/upload_pass");
  settings.push_back("/upload_pass2");
  try {
    store.parseUploadPass(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "upload_pass: invalid number of setting");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}
