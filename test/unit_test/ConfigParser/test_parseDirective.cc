/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parseDirective.cc                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/16 12:33:23 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST
#include "ConfigParser.hpp"
#include "gtest.h"

ConfigParser::DirectiveNode no_setting() {
  ConfigParser::DirectiveNode res;

  res.line_no = 42;
  res.name = "directive_name";
  return res;
}

ConfigParser::DirectiveNode one_setting() {
  ConfigParser::DirectiveNode res = no_setting();
  res.settings.push_back("setting1");
  return res;
}

ConfigParser::DirectiveNode two_setting() {
  ConfigParser::DirectiveNode res = one_setting();
  res.settings.push_back("setting2");
  return res;
}

ConfigParser::DirectiveNode three_setting() {
  ConfigParser::DirectiveNode res = two_setting();
  res.settings.push_back("setting3");
  return res;
}

class test_parseDirective : public ::testing::Test {
 protected:
  ConfigParser parser;

  // 各ケース共通の前処理を書く
  virtual void SetUp() {
    parser.line_count_ = 42;
    parser.char_count_ = strlen("directive_name");
  }

  // 各ケース共通の後処理を書く
  virtual void TearDown() {}
};

TEST_F(test_parseDirective, NoSettingWithoutSpaces) {
  parser.filecontent_ = "directive_name;";
  EXPECT_TRUE(parser.parseDirective("directive_name") == no_setting());
}

TEST_F(test_parseDirective, NoSettingWithSpaces) {
  parser.filecontent_ = "directive_name  \t\v  \f  \n   \r  \n   ;";
  EXPECT_TRUE(parser.parseDirective("directive_name") == no_setting());
}

TEST_F(test_parseDirective, NoSettingFollowedByComment) {
  parser.filecontent_ = "directive_name;#comment";
  EXPECT_TRUE(parser.parseDirective("directive_name") == no_setting());
}

TEST_F(test_parseDirective, NoSettingFollowedByCommentBeforeSemicolon) {
  parser.filecontent_ = "directive_name#comment\n;";
  EXPECT_TRUE(parser.parseDirective("directive_name") == no_setting());
}

TEST_F(test_parseDirective, OneSettingNormal) {
  parser.filecontent_ = "directive_name setting1;";
  EXPECT_TRUE(parser.parseDirective("directive_name") == one_setting());
}

TEST_F(test_parseDirective, OneSettingManySpaces) {
  parser.filecontent_ =
      "directive_name \t\v\f\r\n  \t   setting1 \t\v\f\r\n  \t   ; \t\v\f\r\n  "
      "\t   ";
  EXPECT_TRUE(parser.parseDirective("directive_name") == one_setting());
}

TEST_F(test_parseDirective, OneSettingWithComments) {
  parser.filecontent_ =
      "directive_name# this is a commnent\nsetting1#comment\n;   #commemt\n";
  EXPECT_TRUE(parser.parseDirective("directive_name") == one_setting());
}

TEST_F(test_parseDirective, TwoSettingsNormal) {
  parser.filecontent_ = "directive_name setting1 setting2;";
  EXPECT_TRUE(parser.parseDirective("directive_name") == two_setting());
}

TEST_F(test_parseDirective, TwoSettingsWithManySpaces) {
  parser.filecontent_ =
      "directive_name \t\v\f\r\n  \t   setting1 \t\v\f\rsetting2\n  \t   ; "
      "\t\v\f\r\n  "
      "\t   ";
  EXPECT_TRUE(parser.parseDirective("directive_name") == two_setting());
}

TEST_F(test_parseDirective, TwoSettingWithComments) {
  parser.filecontent_ =
      "directive_name# this is a commnent\nsetting1#comment\nsetting2;   "
      "#commemt\n";
  EXPECT_TRUE(parser.parseDirective("directive_name") == two_setting());
}

TEST_F(test_parseDirective, ThreeSettings) {
  parser.filecontent_ = "directive_name setting1 setting2 setting3;";
  EXPECT_TRUE(parser.parseDirective("directive_name") == three_setting());
}

TEST_F(test_parseDirective, ThreeSettingsGetCrazy) {
  parser.filecontent_ =
      "directive_name\n\n\n#                comment   "
      "\r\r\r\n\n\n\n############\n\n# comment "
      "setting2\n\n\n\n\n\nsetting1#################"
      "setting2\nsetting2\tsetting3\n\v\f\f\f\n#;\n;";
  EXPECT_TRUE(parser.parseDirective("directive_name") == three_setting());
}

TEST_F(test_parseDirective, ErrorNoSemiColonButEOF) {
  parser.filecontent_ = "directive_name";

  ConfigParser::DirectiveNode res;
  try {
    res = parser.parseDirective("directive_name");
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(),
                 "webserv: error: ConfigParser: line 42: expected ';'");
  }
}

TEST_F(test_parseDirective, ErrorNoSemiColonButOpeningBrace) {
  parser.filecontent_ = "directive_name };";

  ConfigParser::DirectiveNode res;
  try {
    res = parser.parseDirective("directive_name");
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(),
                 "webserv: error: ConfigParser: line 42: expected ';'");
  }
}

TEST_F(test_parseDirective, ErrorNoSemiColonButClosingBrace) {
  parser.filecontent_ = "directive_name { hoge };";

  ConfigParser::DirectiveNode res;
  try {
    res = parser.parseDirective("directive_name");
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(),
                 "webserv: error: ConfigParser: line 42: expected ';'");
  }
}
