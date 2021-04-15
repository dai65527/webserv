/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_getIpAddress.cc                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/04/15 11:55:12 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "webserv_utils.hpp"
#include "gtest.h"

class test_getIpAddress : public ::testing::Test {
 protected:
  virtual void SetUp() {}
  virtual void TearDown() {}
};

TEST_F(test_getIpAddress, LittleEndian) {
  EXPECT_EQ(getIpAddress(16777343), "127.0.0.1");
  EXPECT_EQ(getIpAddress(1677830336), "192.168.1.100");
  EXPECT_EQ(getIpAddress(16777215), "255.255.255.0");
  EXPECT_EQ(getIpAddress(-1), "255.255.255.255");
}
