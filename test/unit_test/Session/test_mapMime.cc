/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_mapMime.cc                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/04/10 17:26:11 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "MainConfig.hpp"
#include "Session.hpp"
#include "gtest.h"

class test_mapMine : public ::testing::Test {
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

TEST_F(test_mapMine, testExtToMIME) {
  EXPECT_EQ(Session::map_ext_mime_["html"], "text/html");
  EXPECT_EQ(Session::map_ext_mime_["htm"], "text/html");
  EXPECT_EQ(Session::map_ext_mime_["jpg"], "image/jpeg");
  EXPECT_EQ(Session::map_ext_mime_["jpeg"], "image/jpeg");
  EXPECT_EQ(Session::map_ext_mime_["pdf"], "application/pdf");
}

TEST_F(test_mapMine, testMIMEtoExt) {
  EXPECT_EQ(Session::map_mime_ext_["text/html"], "html");
  EXPECT_EQ(Session::map_mime_ext_["image/jpeg"], "jpg");
  EXPECT_EQ(Session::map_mime_ext_["application/pdf"], "pdf");
}
