/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parseDirective.cc                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/21 09:25:30 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerLocationConfigStore.hpp"
#include "gtest.h"

class test_parseDirective : public ::testing::Test {
 protected:
  bool flg_thrown = false;
  ServerLocationConfigStore store;
  std::list<std::string> settings;

  // 各ケース共通の前処理を書く
  virtual void SetUp() {}

  // 各ケース共通の後処理を書く
  virtual void TearDown() {}
};

TEST_F(test_parseDirective, upload_pass) {
  EXPECT_THROW(store.parseDirective("upload_pass", settings), std::runtime_error);
  settings.push_back("/hoge/fuga");
  EXPECT_TRUE(store.parseDirective("upload_pass", settings));
  EXPECT_EQ(store.getUploadPass(), "/hoge/fuga");
}

TEST_F(test_parseDirective, upload_store) {
  EXPECT_THROW(store.parseDirective("upload_store", settings), std::runtime_error);
  settings.push_back("/hoge/fuga");
  EXPECT_TRUE(store.parseDirective("upload_store", settings));
  EXPECT_EQ(store.getUploadStore(), "/hoge/fuga");
}
