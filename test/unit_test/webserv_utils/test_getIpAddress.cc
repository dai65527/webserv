/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_getIpAddress.cc                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/04/16 22:27:05 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "gtest.h"
#include "webserv_utils.hpp"

class test_getIpAddress : public ::testing::Test {
 protected:
  virtual void SetUp() {}
  virtual void TearDown() {}
};

TEST_F(test_getIpAddress, LittleEndian) {
  EXPECT_EQ(getIpAddress(inet_addr("127.0.0.1")), "127.0.0.1");
  EXPECT_EQ(getIpAddress(inet_addr("192.168.1.100")), "192.168.1.100");
  EXPECT_EQ(getIpAddress(inet_addr("255.255.255.0")), "255.255.255.0");
  EXPECT_EQ(getIpAddress(inet_addr("255.255.255.255")), "255.255.255.255");
}
