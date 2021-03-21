/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parseListen.cc                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/21 09:13:13 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include <arpa/inet.h>

#include "ServerOnlyConfigStore.hpp"
#include "gtest.h"

class test_parseListen : public ::testing::Test {
 protected:
  bool flg_thrown = false;
  ServerOnlyConfigStore store;
  std::list<std::string> settings;

  // 各ケース共通の前処理を書く
  virtual void SetUp() {}

  // 各ケース共通の後処理を書く
  virtual void TearDown() {}
};

TEST_F(test_parseListen, portOnly80) {
  settings.push_back("80");
  EXPECT_NO_THROW(store.parseListen(settings));
  std::list<std::pair<in_addr_t, uint16_t> >::const_iterator itr =
      store.getListen().begin();
  EXPECT_EQ(itr->first, INADDR_ANY);
  EXPECT_EQ(itr->second, htons(80));
}

TEST_F(test_parseListen, portOnly1) {
  settings.push_back("1");
  EXPECT_NO_THROW(store.parseListen(settings));
  std::list<std::pair<in_addr_t, uint16_t> >::const_iterator itr =
      store.getListen().begin();
  EXPECT_EQ(itr->first, INADDR_ANY);
  EXPECT_EQ(itr->second, htons(1));
}

TEST_F(test_parseListen, portOnly65535) {
  settings.push_back("65535");
  EXPECT_NO_THROW(store.parseListen(settings));
  std::list<std::pair<in_addr_t, uint16_t> >::const_iterator itr =
      store.getListen().begin();
  EXPECT_EQ(itr->first, INADDR_ANY);
  EXPECT_EQ(itr->second, htons(65535));
}

TEST_F(test_parseListen, ipOnly127_0_0_1) {
  settings.push_back("127.0.0.1");
  EXPECT_NO_THROW(store.parseListen(settings));
  std::list<std::pair<in_addr_t, uint16_t> >::const_iterator itr =
      store.getListen().begin();
  EXPECT_EQ(itr->first, inet_addr("127.0.0.1"));
  EXPECT_EQ(itr->second, htons(80));
}

TEST_F(test_parseListen, asteriskOnly) {
  settings.push_back("*");
  EXPECT_NO_THROW(store.parseListen(settings));
  std::list<std::pair<in_addr_t, uint16_t> >::const_iterator itr =
      store.getListen().begin();
  EXPECT_EQ(itr->first, INADDR_ANY);
  EXPECT_EQ(itr->second, htons(80));
}

TEST_F(test_parseListen, hostOnly_localhost) {
  settings.push_back("localhost");
  EXPECT_NO_THROW(store.parseListen(settings));
  std::list<std::pair<in_addr_t, uint16_t> >::const_iterator itr =
      store.getListen().begin();
  EXPECT_EQ(itr->first, inet_addr("127.0.0.1"));
  EXPECT_EQ(itr->second, htons(80));
}

TEST_F(test_parseListen, set_twice_1) {
  settings.push_back("localhost");
  EXPECT_NO_THROW(store.parseListen(settings));
  EXPECT_NO_THROW(store.parseListen(settings));
  std::list<std::pair<in_addr_t, uint16_t> >::const_iterator itr =
      store.getListen().begin();
  EXPECT_EQ(itr->first, inet_addr("127.0.0.1"));
  EXPECT_EQ(itr->second, htons(80));
  ++itr;
  EXPECT_EQ(itr, store.getListen().end());
}

TEST_F(test_parseListen, set_twice_2) {
  settings.push_back("localhost");
  EXPECT_NO_THROW(store.parseListen(settings));
  settings.front() = "localhost:80";
  EXPECT_NO_THROW(store.parseListen(settings));
  std::list<std::pair<in_addr_t, uint16_t> >::const_iterator itr =
      store.getListen().begin();
  EXPECT_EQ(itr->first, inet_addr("127.0.0.1"));
  EXPECT_EQ(itr->second, htons(80));
  ++itr;
  EXPECT_EQ(itr, store.getListen().end());
}

TEST_F(test_parseListen, set_twice_3) {
  settings.push_back("80");
  EXPECT_NO_THROW(store.parseListen(settings));
  settings.front() = "80";
  EXPECT_NO_THROW(store.parseListen(settings));
  std::list<std::pair<in_addr_t, uint16_t> >::const_iterator itr =
      store.getListen().begin();
  EXPECT_EQ(itr->first, INADDR_ANY);
  EXPECT_EQ(itr->second, htons(80));
  ++itr;
  EXPECT_EQ(itr, store.getListen().end());
}

TEST_F(test_parseListen, set_twice_4) {
  settings.push_back("80");
  EXPECT_NO_THROW(store.parseListen(settings));
  settings.front() = "localhost";
  EXPECT_NO_THROW(store.parseListen(settings));
  std::list<std::pair<in_addr_t, uint16_t> >::const_iterator itr =
      store.getListen().begin();
  EXPECT_EQ(itr->first, INADDR_ANY);
  EXPECT_EQ(itr->second, htons(80));
  ++itr;
  EXPECT_EQ(itr, store.getListen().end());
}

TEST_F(test_parseListen, set_twice_5) {
  settings.push_back("1.2.3.4:80");
  EXPECT_NO_THROW(store.parseListen(settings));
  settings.front() = "4.3.2.1:80";
  EXPECT_NO_THROW(store.parseListen(settings));
  std::list<std::pair<in_addr_t, uint16_t> >::const_iterator itr =
      store.getListen().begin();
  EXPECT_EQ(itr->first, inet_addr("1.2.3.4"));
  EXPECT_EQ(itr->second, htons(80));
  ++itr;
  EXPECT_EQ(itr->first, inet_addr("4.3.2.1"));
  EXPECT_EQ(itr->second, htons(80));
  ++itr;
  EXPECT_EQ(itr, store.getListen().end());
}

TEST_F(test_parseListen, set_eight_times) {
  settings.push_back("1.2.3.4:80");
  EXPECT_NO_THROW(store.parseListen(settings));
  settings.front() = "4.3.2.1:80";
  EXPECT_NO_THROW(store.parseListen(settings));
  settings.front() = "1.2.3.4:80";
  EXPECT_NO_THROW(store.parseListen(settings));
  settings.front() = "4.3.2.1:42";
  EXPECT_NO_THROW(store.parseListen(settings));
  settings.front() = "42";
  EXPECT_NO_THROW(store.parseListen(settings));
  settings.front() = "8080";
  EXPECT_NO_THROW(store.parseListen(settings));
  settings.front() = "1.2.3.4:8080";
  EXPECT_NO_THROW(store.parseListen(settings));
  settings.front() = "4.2.4.2:4242";
  EXPECT_NO_THROW(store.parseListen(settings));
  std::list<std::pair<in_addr_t, uint16_t> >::const_iterator itr =
      store.getListen().begin();
  EXPECT_EQ(itr->first, inet_addr("1.2.3.4"));
  EXPECT_EQ(itr->second, htons(80));
  ++itr;
  EXPECT_EQ(itr->first, inet_addr("4.3.2.1"));
  EXPECT_EQ(itr->second, htons(80));
  ++itr;
  EXPECT_EQ(itr->first, INADDR_ANY);
  EXPECT_EQ(itr->second, htons(42));
  ++itr;
  EXPECT_EQ(itr->first, INADDR_ANY);
  EXPECT_EQ(itr->second, htons(8080));
  ++itr;
  EXPECT_EQ(itr->first, inet_addr("4.2.4.2"));
  EXPECT_EQ(itr->second, htons(4242));
}

TEST_F(test_parseListen, ipAndPortAsteriskAnd1234) {
  settings.push_back("*:1234");
  EXPECT_NO_THROW(store.parseListen(settings));
  std::list<std::pair<in_addr_t, uint16_t> >::const_iterator itr =
      store.getListen().begin();
  EXPECT_EQ(itr->first, INADDR_ANY);
  EXPECT_EQ(itr->second, htons(1234));
}

TEST_F(test_parseListen, ipAndPort127_0_0_1and1234) {
  settings.push_back("127.0.0.1:1234");
  EXPECT_NO_THROW(store.parseListen(settings));
  std::list<std::pair<in_addr_t, uint16_t> >::const_iterator itr =
      store.getListen().begin();
  EXPECT_EQ(itr->first, inet_addr("127.0.0.1"));
  EXPECT_EQ(itr->second, htons(1234));
}

TEST_F(test_parseListen, hostAndPort_localhost_and1234) {
  settings.push_back("localhost:1234");
  EXPECT_NO_THROW(store.parseListen(settings));
  std::list<std::pair<in_addr_t, uint16_t> >::const_iterator itr =
      store.getListen().begin();
  EXPECT_EQ(itr->first, inet_addr("127.0.0.1"));
  EXPECT_EQ(itr->second, htons(1234));
}

TEST_F(test_parseListen, errorOnlyInvalidPort65536) {
  settings.push_back("65536");
  try {
    store.parseListen(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "listen: invalid port number 65536");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseListen, errorOnlyInvalidPortNeg42) {
  settings.push_back("-42");
  try {
    store.parseListen(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "listen: invalid value \"-42\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseListen, errorInvalidIpOnly256_0_0_0) {
  settings.push_back("256.0.0.0");
  try {
    store.parseListen(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "listen: invalid value \"256.0.0.0\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseListen, errorInvalidHostOnlyHOGE) {
  settings.push_back("HOGE");
  try {
    store.parseListen(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "listen: invalid value \"HOGE\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseListen, errorInvalidIpAndPort_1_2_3_256_80) {
  settings.push_back("1.2.3.256:80");
  try {
    store.parseListen(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "listen: invalid value \"1.2.3.256:80\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseListen, errorInvalidIpAndPort_HOGE_80) {
  settings.push_back("HOGE:80");
  try {
    store.parseListen(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "listen: invalid value \"HOGE:80\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseListen, errorInvalidIpAndPort_127_0_0_1_424242) {
  settings.push_back("127.0.0.1:424242");
  try {
    store.parseListen(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "listen: invalid port number 424242");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseListen, errorInvalidIpAndPort_localhost_424242) {
  settings.push_back("localhost:424242");
  try {
    store.parseListen(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "listen: invalid port number 424242");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseListen, errorInvalidIpAndPort__80_) {
  settings.push_back(":80:");
  try {
    store.parseListen(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "listen: invalid value \":80:\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseListen, errorInvalidIpAndPort_localhost_80_) {
  settings.push_back("localhost:80:");
  try {
    store.parseListen(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "listen: invalid value \"localhost:80:\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseListen, errorInvalidIpAndPort__localhost_80_) {
  settings.push_back(":localhost:80:");
  try {
    store.parseListen(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "listen: invalid value \":localhost:80:\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseListen, errorInvalidIpAndPort__80) {
  settings.push_back(":80");
  try {
    store.parseListen(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "listen: invalid value \":80\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseListen, errorInvalidIpAndPort_localhost__) {
  settings.push_back("localhost:");
  try {
    store.parseListen(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "listen: invalid value \"localhost:\"");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseListen, errorInvalidNumberOfSetting_empty) {
  try {
    store.parseListen(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "listen: invalid number of setting");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}

TEST_F(test_parseListen, errorInvalidNumberOfSetting_2) {
  settings.push_back("localhost");
  settings.push_back("localhost");
  try {
    store.parseListen(settings);
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), "listen: invalid number of setting");
    flg_thrown = true;
  }
  EXPECT_TRUE(flg_thrown);
}
