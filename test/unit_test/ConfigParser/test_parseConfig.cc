/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parseConfig.cc                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/21 19:21:38 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"
#include "gtest.h"

class testThrowError : public ::testing::Test {
 protected:
  ConfigParser parser;
  // 各ケース共通の前処理を書く
  virtual void SetUp() {}

  // 各ケース共通の後処理を書く
  virtual void TearDown() {}
};

TEST_F(testThrowError, nomalConfig) {
  MainConfig config = parser.parseConfig("./configfiles/ok1.conf");

  // check config in main context
  EXPECT_EQ(config.getRoot(), "/main/root");
  EXPECT_EQ(config.getIndex().front(), "main.html");
  EXPECT_EQ(config.getIndex().back(), "main.php");
  EXPECT_EQ(config.getErrorPage().find(HTTP_403)->second, "error_main.html");
  EXPECT_EQ(config.getErrorPage().find(HTTP_404)->second, "error_main.html");
  EXPECT_EQ(config.getAutoIndex(), true);
  EXPECT_EQ(config.getCgiExtension().front(), "cgi");
  EXPECT_EQ(config.getCgiExtension().back(), "main");
  EXPECT_EQ(config.getCharset(), "utf-8");
  EXPECT_EQ(config.getLanguage(), "ja-JP");
  EXPECT_EQ(config.getBaseAuth(), "needpass");
  EXPECT_EQ(config.getClientMaxBodySize(), 42000);
  EXPECT_EQ(config.getLimitExcept(), HTTP_GET | HTTP_HEAD);
  EXPECT_EQ(config.getMaxSessions(), 4242);
  EXPECT_EQ(config.getRetryAfter(), 42);

  // check config in server context
  const ServerConfig& server_config = config.getServers().front();
  EXPECT_EQ(server_config.getListen().front().first, inet_addr("42.42.42.42"));
  EXPECT_EQ(server_config.getListen().front().second, htons(4242));
  EXPECT_EQ(server_config.getServerName().front(), "example.com");
  EXPECT_EQ(server_config.getRoot(), "/server/root");
  EXPECT_EQ(server_config.getIndex().front(), "server.html");
  EXPECT_EQ(server_config.getIndex().back(), "server.php");
  EXPECT_EQ(server_config.getErrorPage().find(HTTP_403)->second,
            "error_server.html");
  EXPECT_EQ(server_config.getErrorPage().find(HTTP_404)->second,
            "error_server.html");
  EXPECT_EQ(server_config.getAutoIndex(), true);
  EXPECT_EQ(server_config.getCgiExtension().front(), "cgi");
  EXPECT_EQ(server_config.getCgiExtension().back(), "server");
  EXPECT_EQ(server_config.getCharset(), "shift-jis");
  EXPECT_EQ(server_config.getLanguage(), "en-US");
  EXPECT_EQ(server_config.getBaseAuth(), "needpass");
  EXPECT_EQ(server_config.getClientMaxBodySize(), 42000000);
  EXPECT_EQ(server_config.getLimitExcept(),
            HTTP_GET | HTTP_HEAD | HTTP_POST);

  // check config in loation context
  const LocationConfig& location_config = server_config.getLocations().front();
  EXPECT_EQ(location_config.getRoute(), "/route/");
  EXPECT_EQ(location_config.getRoot(), "/location/root");
  EXPECT_EQ(location_config.getIndex().front(), "location.html");
  EXPECT_EQ(location_config.getIndex().back(), "location.php");
  EXPECT_EQ(location_config.getErrorPage().find(HTTP_403)->second,
            "error_location.html");
  EXPECT_EQ(location_config.getErrorPage().find(HTTP_404)->second,
            "error_location.html");
  EXPECT_EQ(location_config.getAutoIndex(), false);
  EXPECT_EQ(location_config.getCgiExtension().front(), "cgi");
  EXPECT_EQ(location_config.getCgiExtension().back(), "location");
  EXPECT_EQ(location_config.getCharset(), "iso-8859-1");
  EXPECT_EQ(location_config.getLanguage(), "en-UK");
  EXPECT_EQ(location_config.getBaseAuth(), "needpass");
  EXPECT_EQ(location_config.getClientMaxBodySize(), 42000000);
  EXPECT_EQ(location_config.getLimitExcept(),
            HTTP_GET | HTTP_HEAD | HTTP_POST | HTTP_PUT);

}

TEST_F(testThrowError, errorCannotOpenFile) {
  try {
    parser.parseConfig("filenonexist.conf");
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(),
                 "webserv: error: ConfigParser: filenonexist.conf: "
                 "cannot open file");
  }
}
