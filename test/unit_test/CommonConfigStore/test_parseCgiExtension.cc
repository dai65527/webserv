/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parseCgiExtension.cc                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/18 12:56:30 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gtest.h"
#include "CommonConfigStore.hpp"

class test_parseCgiExtension : public ::testing::Test {
 protected:
  bool flg_thrown = false;
  CommonConfigStore store;
  std::list<std::string> settings;

  // 各ケース共通の前処理を書く
  virtual void SetUp() {}

  // 各ケース共通の後処理を書く
  virtual void TearDown() {}
};

TEST_F(test_parseCgiExtension, normalOne) {
  settings.push_back("cgi");
  EXPECT_NO_THROW(store.parseCgiExtension(settings));
  EXPECT_EQ(store.getCgiExtension(), settings);
}

TEST_F(test_parseCgiExtension, normalThree) {
  settings.push_back("cgi");
  settings.push_back("php");
  settings.push_back("perl");
  EXPECT_NO_THROW(store.parseCgiExtension(settings));
  EXPECT_EQ(store.getCgiExtension(), settings);
}

TEST_F(test_parseCgiExtension, normalOneTwice) {
  settings.push_back("cgi");
  EXPECT_NO_THROW(store.parseCgiExtension(settings));
  settings.front() = "perl";
  EXPECT_NO_THROW(store.parseCgiExtension(settings));
  std::list<std::string>::const_iterator itr = store.getCgiExtension().begin();
  EXPECT_EQ(*itr, "cgi");
  EXPECT_EQ(*++itr, "perl");
}

TEST_F(test_parseCgiExtension, normalOneTwice3times) {
  settings.push_back("cgi");
  EXPECT_NO_THROW(store.parseCgiExtension(settings));
  settings.front() = "perl";
  EXPECT_NO_THROW(store.parseCgiExtension(settings));
  settings.front() = "php";
  EXPECT_NO_THROW(store.parseCgiExtension(settings));
  std::list<std::string>::const_iterator itr = store.getCgiExtension().begin();
  EXPECT_EQ(*itr, "cgi");
  EXPECT_EQ(*++itr, "perl");
  EXPECT_EQ(*++itr, "php");
}

TEST_F(test_parseCgiExtension, settingInvalidNumber) {
  try {
    store.parseCgiExtension(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "cgi_extension: invalid number of setting");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}
