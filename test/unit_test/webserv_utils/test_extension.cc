/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_extension.cc                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/04/20 12:43:58 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "webserv_utils.hpp"
#include "gtest.h"

class test_extension : public ::testing::Test {
 protected:
  virtual void SetUp() {}
  virtual void TearDown() {}
};

TEST_F(test_extension, onlyFileName) {
  EXPECT_EQ(extension(""), "");
  EXPECT_EQ(extension("."), "");
  EXPECT_EQ(extension("hoge"), "");
  EXPECT_EQ(extension("hoge."), "");
  EXPECT_EQ(extension(".hoge"), "");
  EXPECT_EQ(extension("hoge.e"), "e");
  EXPECT_EQ(extension("hoge.ext"), "ext");
  EXPECT_EQ(extension("hoge.extension"), "extension");
}

TEST_F(test_extension, path) {
  EXPECT_EQ(extension("/file/path/hoge"), "");
  EXPECT_EQ(extension("/file/path/hoge."), "");
  EXPECT_EQ(extension("/file/path/.hoge"), "");
  EXPECT_EQ(extension("/file/path/hoge.e"), "e");
  EXPECT_EQ(extension("/file/path/hoge.ext"), "ext");
  EXPECT_EQ(extension("/file/path/hoge.extension"), "extension");
}

TEST_F(test_extension, conplicated) {
  EXPECT_EQ(extension("/file/path/./hoge.ext"), "ext");
  EXPECT_EQ(extension("/file/path/./hoge"), "");
  EXPECT_EQ(extension("/file/path/../hoge.ext"), "ext");
  EXPECT_EQ(extension("/file/path/../hoge"), "");
  EXPECT_EQ(extension("/file.path/hoge.ext"), "ext");
  EXPECT_EQ(extension("/file.path/hoge"), "");
}

TEST_F(test_extension, edgeCases) {
  EXPECT_EQ(extension(""), "");
  EXPECT_EQ(extension("/"), "");
  EXPECT_EQ(extension("./"), "");
  EXPECT_EQ(extension("/."), "");
  EXPECT_EQ(extension("//"), "");
  EXPECT_EQ(extension(".//"), "");
  EXPECT_EQ(extension("/./"), "");
  EXPECT_EQ(extension("//."), "");
  EXPECT_EQ(extension("././."), "");
  EXPECT_EQ(extension("///"), "");
  EXPECT_EQ(extension("///."), "");
  EXPECT_EQ(extension("//./"), "");
  EXPECT_EQ(extension("/.//"), "");
  EXPECT_EQ(extension(".///"), "");
}
