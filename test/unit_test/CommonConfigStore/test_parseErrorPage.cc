/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parseErrorPage.cc                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/17 22:08:21 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "gtest.h"
#include "CommonConfigStore.hpp"

class test_parseErrorPage : public ::testing::Test {
 protected:
  bool flg_thrown = false;
  CommonConfigStore store;
  std::list<std::string> settings;

  // 各ケース共通の前処理を書く
  virtual void SetUp() {}

  // 各ケース共通の後処理を書く
  virtual void TearDown() {}
};

TEST_F(test_parseErrorPage, normal404) {
  settings.push_back("404");
  settings.push_back("/404.html");
  EXPECT_NO_THROW(store.parseErrorPage(settings));
  EXPECT_EQ((*store.getErrorPage().find(HTTP_404)).second, "/404.html");
}

TEST_F(test_parseErrorPage, normal500) {
  settings.push_back("500");
  settings.push_back("/500.html");
  EXPECT_NO_THROW(store.parseErrorPage(settings));
  EXPECT_EQ((*store.getErrorPage().find(HTTP_500)).second, "/500.html");
}

TEST_F(test_parseErrorPage, normal500to504) {
  settings.push_back("500");
  settings.push_back("501");
  settings.push_back("502");
  settings.push_back("503");
  settings.push_back("504");
  settings.push_back("/50x.html");
  EXPECT_NO_THROW(store.parseErrorPage(settings));
  EXPECT_EQ((*store.getErrorPage().find(HTTP_500)).second, "/50x.html");
  EXPECT_EQ((*store.getErrorPage().find(HTTP_501)).second, "/50x.html");
  EXPECT_EQ((*store.getErrorPage().find(HTTP_502)).second, "/50x.html");
  EXPECT_EQ((*store.getErrorPage().find(HTTP_503)).second, "/50x.html");
  EXPECT_EQ((*store.getErrorPage().find(HTTP_504)).second, "/50x.html");
}

TEST_F(test_parseErrorPage, normal403to407and500to504) {
  settings.push_back("403");
  settings.push_back("404");
  settings.push_back("405");
  settings.push_back("406");
  settings.push_back("407");
  settings.push_back("/40x.html");
  EXPECT_NO_THROW(store.parseErrorPage(settings));
  settings.clear();
  settings.push_back("500");
  settings.push_back("501");
  settings.push_back("502");
  settings.push_back("503");
  settings.push_back("504");
  settings.push_back("/50x.html");
  EXPECT_NO_THROW(store.parseErrorPage(settings));
  EXPECT_EQ((*store.getErrorPage().find(HTTP_403)).second, "/40x.html");
  EXPECT_EQ((*store.getErrorPage().find(HTTP_404)).second, "/40x.html");
  EXPECT_EQ((*store.getErrorPage().find(HTTP_405)).second, "/40x.html");
  EXPECT_EQ((*store.getErrorPage().find(HTTP_406)).second, "/40x.html");
  EXPECT_EQ((*store.getErrorPage().find(HTTP_407)).second, "/40x.html");
  EXPECT_EQ((*store.getErrorPage().find(HTTP_500)).second, "/50x.html");
  EXPECT_EQ((*store.getErrorPage().find(HTTP_501)).second, "/50x.html");
  EXPECT_EQ((*store.getErrorPage().find(HTTP_502)).second, "/50x.html");
  EXPECT_EQ((*store.getErrorPage().find(HTTP_503)).second, "/50x.html");
  EXPECT_EQ((*store.getErrorPage().find(HTTP_504)).second, "/50x.html");
}

TEST_F(test_parseErrorPage, errorNoSetting) {
  try {
    store.parseErrorPage(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "error_page: invalid number of setting");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseErrorPage, errorSettingOnlyOneSetting) {
  settings.push_back("404.html");
  try {
    store.parseErrorPage(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "error_page: invalid number of setting");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseErrorPage, errorSettingInvalidNumberNeg404) {
  settings.push_back("-404");
  settings.push_back("404.html");
  try {
    store.parseErrorPage(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "error_page: invalid value \"-404\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseErrorPage, errorSettingInvalidNumber200) {
  settings.push_back("200");
  settings.push_back("404.html");
  try {
    store.parseErrorPage(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "error_page: value must be between 300 and 599");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseErrorPage, errorSettingInvalidNumber600) {
  settings.push_back("600");
  settings.push_back("404.html");
  try {
    store.parseErrorPage(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "error_page: invalid value \"600\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseErrorPage, errorSettingInvalid500_200_201_501) {
  settings.push_back("500");
  settings.push_back("200");
  settings.push_back("201");
  settings.push_back("501");
  settings.push_back("/50x.html");
  try {
    store.parseErrorPage(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "error_page: value must be between 300 and 599");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseErrorPage, errorSettingInvalid500_42_4200_501) {
  settings.push_back("500");
  settings.push_back("42");
  settings.push_back("4200");
  settings.push_back("501");
  settings.push_back("/50x.html");
  try {
    store.parseErrorPage(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "error_page: invalid value \"42\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}
