/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_getFileExtension.cc                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/04/10 23:05:02 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "MainConfig.hpp"
#include "Session.hpp"
#include "gtest.h"

class test_getFileExtension : public ::testing::Test {
 protected:
  MainConfig config;
  Session* session;
  bool flg_thrown;

  virtual void SetUp() {
    flg_thrown = false;
    session = new Session(0, config);
  }

  // 各ケース共通の後処理を書く
  virtual void TearDown() { delete session; }
};

TEST_F(test_getFileExtension, normal_html) {
  session->request_.headers_["content-type"] = "text/html";
  EXPECT_EQ(session->getFileExtension(), "html");
}

TEST_F(test_getFileExtension, normal_3gppaudio) {
  session->request_.headers_["content-type"] = "audio/3gpp";
  EXPECT_EQ(session->getFileExtension(), "3gp");
}

TEST_F(test_getFileExtension, normal_3gppvideo) {
  session->request_.headers_["content-type"] = "video/3gpp";
  EXPECT_EQ(session->getFileExtension(), "3gp");
}

TEST_F(test_getFileExtension, no_content_type_header) {
  EXPECT_EQ(session->getFileExtension(), "");
}

TEST_F(test_getFileExtension, no_such_mime) {
  session->request_.headers_["content-type"] = "hoge/fuga";
  EXPECT_EQ(session->getFileExtension(), "");
}

TEST_F(test_getFileExtension, withcharset1_html) {
  session->request_.headers_["content-type"] = "text/html; charset=UTF-8";
  EXPECT_EQ(session->getFileExtension(), "html");
}

TEST_F(test_getFileExtension, withcharset2_html) {
  session->request_.headers_["content-type"] = "text/html ;charset=UTF-8";
  EXPECT_EQ(session->getFileExtension(), "html");
}
