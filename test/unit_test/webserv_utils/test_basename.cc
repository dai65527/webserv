/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_basename.cc                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/04/11 11:43:25 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "webserv_utils.hpp"
#include "gtest.h"

class test_findServer : public ::testing::Test {
 protected:
  virtual void SetUp() {}
  virtual void TearDown() {}
};

TEST_F(test_findServer, normalCase) {
  EXPECT_EQ(basename("/hoge/fuga"), "fuga");
  EXPECT_EQ(basename("/hoge/fuga/"), "fuga");
  EXPECT_EQ(basename("///hoge///fuga///"), "fuga");
  EXPECT_EQ(basename("/hogehoge/fugafuga/hoge/fuga"), "fuga");
  EXPECT_EQ(basename("/hoge//fuga"), "fuga");
  EXPECT_EQ(basename("/hoge/fuga//"), "fuga");
}

TEST_F(test_findServer, edgeCases) {
  EXPECT_EQ(basename(""), "");
  EXPECT_EQ(basename("/"), "/");
  EXPECT_EQ(basename("//"), "/");
  EXPECT_EQ(basename("//////"), "/");
  EXPECT_EQ(basename("/hoge"), "hoge");
  EXPECT_EQ(basename("/h"), "h");
  EXPECT_EQ(basename("hoge"), "hoge");
  EXPECT_EQ(basename("h"), "h");
  EXPECT_EQ(basename("hoge/"), "hoge");
  EXPECT_EQ(basename("h/"), "h");
  EXPECT_EQ(basename("/hoge/"), "hoge");
  EXPECT_EQ(basename("/h/"), "h");
}
