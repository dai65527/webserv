/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_isLocationMatch.cc                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/26 11:25:44 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "MainConfig.hpp"
#include "Session.hpp"
#include "gtest.h"

class test_isLocationMatch : public ::testing::Test {
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

TEST_F(test_isLocationMatch, allCompleteMatch) {
  EXPECT_TRUE(session->isLocationMatch("/", "/"));
  EXPECT_TRUE(session->isLocationMatch("/hoge", "/hoge"));
  EXPECT_TRUE(session->isLocationMatch("/hoge/", "/hoge/"));
  EXPECT_TRUE(session->isLocationMatch("/hoge/fuga", "/hoge/fuga"));
  EXPECT_TRUE(session->isLocationMatch("/hoge/fuga/", "/hoge/fuga/"));
  EXPECT_TRUE(
      session->isLocationMatch("/hoge/fuga/42tokyo/", "/hoge/fuga/42tokyo/"));
}

TEST_F(test_isLocationMatch, partialMatch) {
  EXPECT_TRUE(session->isLocationMatch("/", "/hoge"));
  EXPECT_TRUE(session->isLocationMatch("/", "/hoge/"));
  EXPECT_TRUE(session->isLocationMatch("/hoge", "/hoge/fuga"));
  EXPECT_TRUE(session->isLocationMatch("/hoge", "/hoge/fuga/"));
  EXPECT_TRUE(session->isLocationMatch("/hoge/", "/hoge/fuga"));
  EXPECT_TRUE(session->isLocationMatch("/hoge/", "/hoge/fuga/"));
}

TEST_F(test_isLocationMatch, notMatch) {
  EXPECT_FALSE(session->isLocationMatch("/fuga/", "/hoge"));
  EXPECT_FALSE(session->isLocationMatch("/fuga/", "/hoge/"));
  EXPECT_FALSE(session->isLocationMatch("/fuga/", "/"));
  EXPECT_FALSE(session->isLocationMatch("/fuga/", "/"));
  EXPECT_FALSE(session->isLocationMatch("/hoge/", "/hogefuga"));
  EXPECT_FALSE(session->isLocationMatch("/hoge/", "/hogefuga/"));
  EXPECT_FALSE(session->isLocationMatch("/hogefuga", "/hoge/fuga"));
  EXPECT_FALSE(session->isLocationMatch("/hogefuga", "/hoge/fuga/"));
}
