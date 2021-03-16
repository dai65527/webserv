/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parseLocation.cc                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/16 19:01:56 by dnakano          ###   ########.fr       */
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
    res.settings.push_back("setting" + std::to_string(i));
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

  for (int i = 0; i < n; i++) {
    res.directives.push_back(directive(i, line_no));
  }
  return res;
}

static ConfigParser::LocationContextNode locationMultiLine(int n, int line_no) {
  ConfigParser::LocationContextNode res = locationEmpty(line_no);

  for (int i = 0; i < n; i++) {
    res.directives.push_back(directive(i, ++line_no));
  }
  return res;
}

static ConfigParser::LocationContextNode locationMultiAndEmptyLine(
    int n, int line_no) {
  ConfigParser::LocationContextNode res = locationEmpty(line_no);

  for (int i = 0; i < n; i++) {
    res.directives.push_back(directive(i, ++++line_no));
  }
  return res;
}

class test_parseLocation : public ::testing::Test {
 protected:
  ConfigParser parser;

  // 各ケース共通の前処理を書く
  virtual void SetUp() {
    parser.line_count_ = 1;
    parser.char_count_ = strlen("location");
  }

  // 各ケース共通の後処理を書く
  virtual void TearDown() {}
};

TEST_F(test_parseLocation, NoDirectiveOneLine) {
  parser.filecontent_ = "location /route/ {}";
  EXPECT_TRUE(locationEmpty(parser.line_count_) ==
              parser.parseLocationContext());
}

TEST_F(test_parseLocation, NoDirectiveMultiLine) {
  parser.filecontent_ = "location \n /route/ \r\n {\n\n}\n\n";
  EXPECT_TRUE(locationEmpty(parser.line_count_) ==
              parser.parseLocationContext());
}

TEST_F(test_parseLocation, OneDirectiveOneLine) {
  parser.filecontent_ = "location /route/ { directive0; }";
  EXPECT_TRUE(locationOneLine(1, parser.line_count_) ==
              parser.parseLocationContext());
}

TEST_F(test_parseLocation, OneDirectiveMultiLine) {
  parser.filecontent_ = "location /route/ {\n\tdirective0;\n}\n";
  EXPECT_TRUE(locationMultiLine(1, parser.line_count_) ==
              parser.parseLocationContext());
}

TEST_F(test_parseLocation, OneDirectiveMultiAndEmptyLine) {
  parser.filecontent_ = "location /route/\n{\n\tdirective0;\n}\n";
  EXPECT_TRUE(locationMultiAndEmptyLine(1, parser.line_count_) ==
              parser.parseLocationContext());
}

TEST_F(test_parseLocation, TwoDirectiveOneLine) {
  parser.filecontent_ = "location /route/ { directive0; directive1 setting0;}";
  EXPECT_TRUE(locationOneLine(2, parser.line_count_) ==
              parser.parseLocationContext());
}

TEST_F(test_parseLocation, TwoDirectiveMultiLine) {
  parser.filecontent_ =
      "location /route/ {\n\tdirective0;\n\tdirective1 setting0;}";
  EXPECT_TRUE(locationMultiLine(2, parser.line_count_) ==
              parser.parseLocationContext());
}

TEST_F(test_parseLocation, TwoDirectiveMultiAndEmptyLine) {
  parser.filecontent_ =
      "location /route/\n{\n\tdirective0;\n\n\tdirective1 setting0;}";
  EXPECT_TRUE(locationMultiAndEmptyLine(2, parser.line_count_) ==
              parser.parseLocationContext());
}

TEST_F(test_parseLocation, ThreeDirectiveOneLine) {
  parser.filecontent_ =
      "location /route/ { directive0; directive1 setting0;directive2 setting0 "
      "setting1 ;}";
  EXPECT_TRUE(locationOneLine(3, parser.line_count_) ==
              parser.parseLocationContext());
}

TEST_F(test_parseLocation, ThreeDirectiveMultiLine) {
  parser.filecontent_ =
      "location /route/ {\n\tdirective0;\n\tdirective1 setting0;\ndirective2 "
      "setting0 setting1;}";
  EXPECT_TRUE(locationMultiLine(3, parser.line_count_) ==
              parser.parseLocationContext());
}

TEST_F(test_parseLocation, ThreeDirectiveMultiAndEmptyLine) {
  parser.filecontent_ =
      "location /route/\n{\n\tdirective0;\n\n\tdirective1 setting0;\n     "
      "\ndirective2 setting0   \n\rsetting1;}";
  EXPECT_TRUE(locationMultiAndEmptyLine(3, parser.line_count_) ==
              parser.parseLocationContext());
}

TEST_F(test_parseLocation, FiveDirectiveOneLine) {
  parser.filecontent_ =
      "location /route/ { directive0; directive1 setting0;directive2 setting0 "
      "setting1 ; directive3 setting0 setting1 setting2; directive4 setting0 "
      "setting1 setting2 setting3;}";
  EXPECT_TRUE(locationOneLine(5, parser.line_count_) ==
              parser.parseLocationContext());
}

TEST_F(test_parseLocation, FiveDirectiveMultiLine) {
  parser.filecontent_ =
      "location /route/ {\n"
      "directive0;\n"
      "directive1 setting0;\n"
      "directive2 setting0 setting1 ;\n   "
      "directive3 setting0 setting1 setting2;   \n"
      "directive4 setting0 setting1 setting2 setting3; }\n  ";
  EXPECT_TRUE(locationMultiLine(5, parser.line_count_) ==
              parser.parseLocationContext());
}

TEST_F(test_parseLocation, FiveDirectiveMultiAndEmptyLine) {
  parser.filecontent_ =
      "location /route/\n {\n"
      "directive0;\n\n"
      "directive1 setting0\n;\n"
      "directive2 setting0\nsetting1 ;\n   "
      "directive3 setting0 setting1 \nsetting2;   \n"
      "directive4 setting0 setting1 setting2 setting3;\n\n }\n  ";
  EXPECT_TRUE(locationMultiAndEmptyLine(5, parser.line_count_) ==
              parser.parseLocationContext());
}

TEST_F(test_parseLocation, ErrorNoLocationRoute) {
  parser.filecontent_ =
      "location {\n\tdirective0;\n\tdirective1 setting0;\ndirective2 "
      "setting0 setting1;}";
  try {
    parser.parseLocationContext();
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(),
                 "webserv: error: ConfigParser: line 1: expected route config "
                 "of location directive");
  }
}

TEST_F(test_parseLocation, ErrorNoOpeningBrace) {
  parser.filecontent_ =
      "location /route/ \n\tdirective0;\n\tdirective1 setting0;\ndirective2 "
      "setting0 setting1;}";
  try {
    parser.parseLocationContext();
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(),
                 "webserv: error: ConfigParser: line 2: expected '{'");
  }
}

TEST_F(test_parseLocation, ErrorNoClosingBrace) {
  parser.filecontent_ =
      "location /route/ {\n\tdirective0;\n\tdirective1 setting0;\ndirective2 "
      "setting0 setting1;";
  try {
    parser.parseLocationContext();
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(),
                 "webserv: error: ConfigParser: line 4: expected '}'");
  }
}

TEST_F(test_parseLocation, ErrorNoSemicolon) {
  parser.filecontent_ =
      "location /route/ {\n\tdirective0;\n\tdirective1 setting0;\ndirective2 "
      "setting0 setting1};";
  try {
    parser.parseLocationContext();
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(),
                 "webserv: error: ConfigParser: line 4: expected ';'");
  }
}

TEST_F(test_parseLocation, ErrorHasDirective) {
  parser.filecontent_ =
      "location /route/ {\n\tdirective0;\n {setting0;\ndirective2 "
      "setting0 setting1};";
  try {
    parser.parseLocationContext();
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(
        e.what(),
                 "webserv: error: ConfigParser: line 3: expected '}'");
  }
}

TEST_F(test_parseLocation, ErrorHasServerConfig) {
  parser.filecontent_ =
      "location /route/ {\n\tdirective0;\nserver{ setting0;}\ndirective2 "
      "setting0 setting1};";
  try {
    parser.parseLocationContext();
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(
        e.what(),
        "webserv: error: ConfigParser: line 3: unexpected server directive");
  }
}

TEST_F(test_parseLocation, ErrorHasLocationConfig) {
  parser.filecontent_ =
      "location /route/ {\n\tdirective0;\nlocation /route/ { "
      "setting0;}\ndirective2 "
      "setting0 setting1};";
  try {
    parser.parseLocationContext();
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(
        e.what(),
        "webserv: error: ConfigParser: line 3: unexpected location directive");
  }
}
