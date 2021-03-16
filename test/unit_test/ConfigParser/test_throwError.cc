/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_throwError.cc                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/16 12:12:30 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"
#include "gtest.h"

class testThrowError : public ::testing::Test {
 protected:
  // 各ケース共通の前処理を書く
  virtual void SetUp() { std::cout << "setup" << std::endl; }

  // 各ケース共通の後処理を書く
  virtual void TearDown() { std::cout << "tear down" << std::endl; }
};

TEST(testThrowError, cannotOpenFile) {
  try {
    ConfigParser parser("filenonexist.conf");
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(),
                 "webserv: error: ConfigParser: filenonexist.conf: "
                 "cannot open file");
  }
}
