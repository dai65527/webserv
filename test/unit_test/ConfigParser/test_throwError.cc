/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_throwError.cc                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/15 19:58:59 by dnakano          ###   ########.fr       */
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
  } catch (std::runtime_error e) {
    EXPECT_EQ(0,
              strcmp(e.what(),
                     "webserv: config: filenonexist.conf: cannot open file"));
  }
}
