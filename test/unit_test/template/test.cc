/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cc                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/09 18:30:11 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gtest.h"
#include "target.hpp"

class fixtureName : public ::testing::Test {
 protected:
  // 各ケース共通の前処理を書く
  virtual void SetUp() {}

  // 各ケース共通の後処理を書く
  virtual void TearDown() {}
};

TEST_F(fixtureName, testOk) {
  EXPECT_EQ(true, isOdd(21));
  EXPECT_EQ(false, isOdd(42));
  EXPECT_EQ(false, isEven(21));
  EXPECT_EQ(true, isEven(42));
}

TEST_F(fixtureName, testNg) {
  EXPECT_EQ(true, isOdd(21));
  EXPECT_EQ(true, isOdd(42));  // this fails
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
