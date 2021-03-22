/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parseDirective.cc                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/21 10:20:36 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MainOnlyConfigStore.hpp"
#include "gtest.h"

class test_parseDirective : public ::testing::Test {
 protected:
  bool flg_thrown = false;
  MainOnlyConfigStore store;
  std::list<std::string> settings;

  // 各ケース共通の前処理を書く
  virtual void SetUp() {}

  // 各ケース共通の後処理を書く
  virtual void TearDown() {}
};

TEST_F(test_parseDirective, max_sessions) {
  EXPECT_THROW(store.parseDirective("max_sessions", settings),
               std::runtime_error);
  settings.push_back("4242");
  EXPECT_TRUE(store.parseDirective("max_sessions", settings));
  EXPECT_EQ(store.getMaxSessions(), 4242);
}

TEST_F(test_parseDirective, retry_after) {
  EXPECT_THROW(store.parseDirective("retry_after", settings),
               std::runtime_error);
  settings.push_back("4242");
  EXPECT_TRUE(store.parseDirective("retry_after", settings));
  EXPECT_EQ(store.getRetryAfter(), 4242);
}
