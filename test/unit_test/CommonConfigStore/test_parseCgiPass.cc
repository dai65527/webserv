/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parseCgiPass.cc                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/04/30 21:13:21 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "gtest.h"
#include "CommonConfigStore.hpp"

class test_parseCgiPass : public ::testing::Test {
 protected:
  bool flg_thrown = false;
  CommonConfigStore store;
  std::list<std::string> settings;

  // 各ケース共通の前処理を書く
  virtual void SetUp() {}

  // 各ケース共通の後処理を書く
  virtual void TearDown() {}
};

TEST_F(test_parseCgiPass, normal) {
  settings.push_back("./path/to/cgifile");
  EXPECT_NO_THROW(store.parseCgiPass(settings));
  EXPECT_EQ(store.getCgiPass(), settings.front());
}

TEST_F(test_parseCgiPass, emptySetting) {
  try {
    store.parseCgiPass(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "cgi_pass: invalid number of setting");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseCgiPass, duplicateSetting) {
  settings.push_back("./path/to/cgifile");
  try {
    store.parseCgiPass(settings);
    store.parseCgiPass(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "cgi_pass: directive duplicated");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseCgiPass, settingInvalidNumber) {
  settings.push_back("./path/to/cgifile");
  settings.push_back("./path/to/cgifile");
  try {
    store.parseCgiPass(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "cgi_pass: invalid number of setting");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}
