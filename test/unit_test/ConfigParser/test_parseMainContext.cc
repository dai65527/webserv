/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parseMainContext.cc                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/17 18:09:09 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST
#include "ConfigParser.hpp"
#include "gtest.h"

static ConfigParser::DirectiveNode directive(int n, size_t line_no) {
  ConfigParser::DirectiveNode res;
  res.line_no = line_no;
  res.name = "directive" + std::to_string(n);
  for (int i = 0; i < n; i++) {
    res.settings.push_back("setting" + std::to_string(i + 1));
  }
  return res;
}

static ConfigParser::LocationContextNode locationEmpty(int line_no) {
  ConfigParser::LocationContextNode res;

  res.line_no = line_no;
  res.route = "/route/";
  return res;
}

static ConfigParser::LocationContextNode locationOneLine(int n, int line_no) {
  ConfigParser::LocationContextNode res = locationEmpty(line_no);

  for (int i = 0; i <= n; i++) {
    res.directives.push_back(directive(i, line_no));
  }
  return res;
}

static ConfigParser::LocationContextNode locationMultiLine(int n, int line_no) {
  ConfigParser::LocationContextNode res = locationEmpty(line_no);

  for (int i = 0; i <= n; i++) {
    res.directives.push_back(directive(i, ++line_no));
  }
  return res;
}

static ConfigParser::LocationContextNode locationMultiAndEmptyLine(
    int n, int line_no) {
  ConfigParser::LocationContextNode res = locationEmpty(line_no);

  for (int i = 0; i <= n; i++) {
    res.directives.push_back(directive(i, ++++line_no));
  }
  return res;
}

static ConfigParser::ServerContextNode serverEmpty(int line_no) {
  ConfigParser::ServerContextNode res;

  res.line_no = line_no;
  return res;
}

static ConfigParser::ServerContextNode serverOneLine(int n, int line_no) {
  ConfigParser::ServerContextNode res = serverEmpty(line_no);

  for (int i = 0; i <= n; i++) {
    res.directives.push_back(directive(i, line_no));
  }
  return res;
}

static ConfigParser::ServerContextNode serverMultiLine(int n, int line_no) {
  ConfigParser::ServerContextNode res = serverEmpty(line_no);

  for (int i = 0; i <= n; i++) {
    res.directives.push_back(directive(i, ++line_no));
  }
  return res;
}

static ConfigParser::ServerContextNode serverMultiAndEmptyLine(int n,
                                                               int line_no) {
  ConfigParser::ServerContextNode res = serverEmpty(line_no);

  for (int i = 0; i <= n; i++) {
    res.directives.push_back(directive(i, ++++line_no));
  }
  return res;
}

class test_parseMainContext : public ::testing::Test {
 protected:
  ConfigParser parser;

  // 各ケース共通の前処理を書く
  virtual void SetUp() {
    parser.line_count_ = 1;
    parser.char_count_ = 0;
  }

  // 各ケース共通の後処理を書く
  virtual void TearDown() {}
};

TEST_F(test_parseMainContext, Empty) {
  parser.filecontent_ = "";
  ConfigParser::MainContextNode res = parser.parseMainContext();
  ConfigParser::MainContextNode exp;

  EXPECT_TRUE(res == exp);
}

TEST_F(test_parseMainContext, ThreeEmptyServer) {
  parser.filecontent_ =
      "server{}"
      "server{}"
      "server{}";

  ConfigParser::MainContextNode exp;
  size_t line_no = parser.line_count_;
  exp.servers.push_back(serverEmpty(parser.line_count_));
  exp.servers.push_back(serverEmpty(parser.line_count_));
  exp.servers.push_back(serverEmpty(parser.line_count_));

  ConfigParser::MainContextNode res = parser.parseMainContext();

  EXPECT_TRUE(res == exp);
}

TEST_F(test_parseMainContext, OneDirective) {
  parser.filecontent_ = "directive1 setting1;";

  ConfigParser::MainContextNode exp;
  size_t line_no = parser.line_count_;
  exp.directives.push_back(directive(1, line_no));

  ConfigParser::MainContextNode res = parser.parseMainContext();

  EXPECT_TRUE(res == exp);
}

TEST_F(test_parseMainContext, ThreeDirective) {
  parser.filecontent_ =
      "directive0;"
      "directive1 setting1;"
      "directive2 setting1 setting2;";

  ConfigParser::MainContextNode exp;
  size_t line_no = parser.line_count_;
  exp.directives.push_back(directive(0, line_no));
  exp.directives.push_back(directive(1, line_no));
  exp.directives.push_back(directive(2, line_no));

  ConfigParser::MainContextNode res = parser.parseMainContext();

  EXPECT_TRUE(res == exp);
}

TEST_F(test_parseMainContext, ThreeDirectiveAndThreeServers) {
  parser.filecontent_ =
      "directive0;"
      "server {directive0; directive1 setting1;}"
      "directive1 setting1;"
      "server {directive0; directive1 setting1; directive2 setting1 setting2;}"
      "server {directive0;}"
      "directive2 setting1 setting2;";

  ConfigParser::MainContextNode exp;
  size_t line_no = parser.line_count_;
  exp.directives.push_back(directive(0, line_no));
  exp.directives.push_back(directive(1, line_no));
  exp.directives.push_back(directive(2, line_no));
  exp.servers.push_back(serverOneLine(1, line_no));
  exp.servers.push_back(serverOneLine(2, line_no));
  exp.servers.push_back(serverOneLine(0, line_no));

  ConfigParser::MainContextNode res = parser.parseMainContext();

  EXPECT_TRUE(res == exp);
}

TEST_F(test_parseMainContext, ThreeDirectiveAndThreeServersWithNewLines) {
  parser.filecontent_ =
      "directive0;\n"
      "server {directive0; directive1 setting1;}\n"
      "directive1 setting1;\n"
      "server {directive0; directive1 setting1; directive2 setting1 "
      "setting2;\n}"
      "server {directive0;}\n"
      "directive2 setting1\n setting2;";

  ConfigParser::MainContextNode exp;
  size_t line_no = parser.line_count_;
  exp.directives.push_back(directive(0, line_no++));
  exp.servers.push_back(serverOneLine(1, line_no++));
  exp.directives.push_back(directive(1, line_no++));
  exp.servers.push_back(serverOneLine(2, line_no++));
  exp.servers.push_back(serverOneLine(0, line_no++));
  exp.directives.push_back(directive(2, line_no++));

  ConfigParser::MainContextNode res = parser.parseMainContext();

  EXPECT_TRUE(res == exp);
}

TEST_F(test_parseMainContext, ThreeDirectiveAndThreeServersWithSpaces) {
  parser.filecontent_ =
      "    \t\r  \t\f      directive0;\n"
      "server {directive0  \t\r  \t\f    ;  \t\r  \t\f     directive1 "
      "setting1;  \t\r  \t\f    }\n"
      "directive1 setting1;\n  \t\r  \t\f    "
      "server {directive0;   \t\r  \t\f    directive1 setting1; directive2 setting1 "
      "setting2;\n}  \t\r  \t\f    "
      "server   \t\r  \t\f    {  \t\r  \t\f    directive0;}  \t\r  \t\f    \n"
      "directive2 setting1\n   \t\r  \t\f    setting2;";

  ConfigParser::MainContextNode exp;
  size_t line_no = parser.line_count_;
  exp.directives.push_back(directive(0, line_no++));
  exp.servers.push_back(serverOneLine(1, line_no++));
  exp.directives.push_back(directive(1, line_no++));
  exp.servers.push_back(serverOneLine(2, line_no++));
  exp.servers.push_back(serverOneLine(0, line_no++));
  exp.directives.push_back(directive(2, line_no++));

  ConfigParser::MainContextNode res = parser.parseMainContext();

  EXPECT_TRUE(res == exp);
}

TEST_F(test_parseMainContext, ErrorUnexpedtedLocation) {
  parser.filecontent_ =
      "directive0;\n"
      "location /route/ {directive0; directive1 setting1;}"
      "directive1 setting1;"
      "server {directive0; directive1 setting1; directive2 setting1 setting2;}"
      "server {directive0;}"
      "directive2 setting1 setting2;";

  try {
    parser.parseMainContext();
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(
        e.what(),
        "webserv: error: ConfigParser: line 2: unexpected location directive");
  }
}

TEST_F(test_parseMainContext, ErrorUnexpedtedSemicolon) {
  parser.filecontent_ =
      "directive0;\n"
      "server {directive0; directive1 setting1;}\n"
      ";directive1 setting1;\n"
      "server {directive0; directive1 setting1; directive2 setting1 setting2;}"
      "server {directive0;}"
      "directive2 setting1 setting2;";

  try {
    parser.parseMainContext();
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(
        e.what(),
        "webserv: error: ConfigParser: line 3: unexpected ';'");
  }
}

TEST_F(test_parseMainContext, ErrorUnexpedtedOpeningBrace) {
  parser.filecontent_ =
      "directive0;\n"
      "server {directive0; directive1 setting1;}\n"
      "{directive1 setting1;\n"
      "server {directive0; directive1 setting1; directive2 setting1 setting2;}"
      "server {directive0;}"
      "directive2 setting1 setting2;";

  try {
    parser.parseMainContext();
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(
        e.what(),
        "webserv: error: ConfigParser: line 3: unexpected '{'");
  }
}
