/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parseDirective.cc                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/04/28 15:54:02 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "CommonConfigStore.hpp"
#include "gtest.h"

class test_parseDirective : public ::testing::Test {
 protected:
  bool flg_thrown = false;
  CommonConfigStore store;
  std::list<std::string> settings;

  // 各ケース共通の前処理を書く
  virtual void SetUp() {}

  // 各ケース共通の後処理を書く
  virtual void TearDown() {}
};

TEST_F(test_parseDirective, root) {
  EXPECT_THROW(store.parseDirective("root", settings), std::runtime_error);
  settings.push_back("/root");
  EXPECT_TRUE(store.parseDirective("root", settings));
  EXPECT_EQ(store.getRoot(), "/root");
}

TEST_F(test_parseDirective, index) {
  EXPECT_THROW(store.parseDirective("index", settings), std::runtime_error);
  settings.push_back("index.html");
  EXPECT_TRUE(store.parseDirective("index", settings));
  EXPECT_EQ(store.getIndex().front(), "index.html");
}

TEST_F(test_parseDirective, error_page) {
  EXPECT_THROW(store.parseDirective("error_page", settings),
               std::runtime_error);
  settings.push_back("404");
  settings.push_back("404.html");
  EXPECT_TRUE(store.parseDirective("error_page", settings));
  EXPECT_EQ(store.getErrorPage().find(HTTP_404)->second, "404.html");
}

TEST_F(test_parseDirective, autoindex) {
  EXPECT_THROW(store.parseDirective("autoindex", settings), std::runtime_error);
  settings.push_back("on");
  EXPECT_TRUE(store.parseDirective("autoindex", settings));
  EXPECT_EQ(store.getAutoIndex(), true);
}

TEST_F(test_parseDirective, cgi_extension) {
  EXPECT_THROW(store.parseDirective("cgi_extension", settings),
               std::runtime_error);
  settings.push_back("cgi");
  EXPECT_TRUE(store.parseDirective("cgi_extension", settings));
  EXPECT_EQ(store.getCgiExtension().front(), "cgi");
}

TEST_F(test_parseDirective, charset) {
  EXPECT_THROW(store.parseDirective("charset", settings), std::runtime_error);
  settings.push_back("utf-8");
  EXPECT_TRUE(store.parseDirective("charset", settings));
  EXPECT_EQ(store.getCharset(), "utf-8");
}

TEST_F(test_parseDirective, language) {
  EXPECT_THROW(store.parseDirective("language", settings), std::runtime_error);
  settings.push_back("ja-JP");
  EXPECT_TRUE(store.parseDirective("language", settings));
  EXPECT_EQ(store.getLanguage().front(), "ja-JP");
}

TEST_F(test_parseDirective, base_auth) {
  EXPECT_THROW(store.parseDirective("base_auth", settings), std::runtime_error);
  settings.push_back("message");
  EXPECT_TRUE(store.parseDirective("base_auth", settings));
  EXPECT_EQ(store.getBaseAuth(), "message");
}

TEST_F(test_parseDirective, auth_basic_user_file) {
  EXPECT_THROW(store.parseDirective("auth_basic_user_file", settings),
               std::runtime_error);
  settings.push_back("auth_basic_user_file.html");
  EXPECT_TRUE(store.parseDirective("auth_basic_user_file", settings));
  EXPECT_EQ(store.getAuthBasicUserFile().front(), "auth_basic_user_file.html");
}

TEST_F(test_parseDirective, client_max_body_size) {
  EXPECT_THROW(store.parseDirective("client_max_body_size", settings),
               std::runtime_error);
  settings.push_back("42");
  EXPECT_TRUE(store.parseDirective("client_max_body_size", settings));
  EXPECT_EQ(store.getClientMaxBodySize(), 42);
}

TEST_F(test_parseDirective, limit_except) {
  EXPECT_THROW(store.parseDirective("limit_except", settings),
               std::runtime_error);
  settings.push_back("GET");
  EXPECT_TRUE(store.parseDirective("limit_except", settings));
  EXPECT_EQ(store.getLimitExcept(), HTTP_GET);
}
