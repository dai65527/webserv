/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parseServer.cc                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/17 13:10:03 by dnakano          ###   ########.fr       */
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

class test_parseServer : public ::testing::Test {
 protected:
  ConfigParser parser;

  // 各ケース共通の前処理を書く
  virtual void SetUp() {
    parser.line_count_ = 1;
    parser.char_count_ = strlen("server");
  }

  // 各ケース共通の後処理を書く
  virtual void TearDown() {}
};

TEST_F(test_parseServer, NoDirectiveOneLine) {
  parser.filecontent_ = "server {}";
  EXPECT_TRUE(serverEmpty(parser.line_count_) == parser.parseServerContext());
}

TEST_F(test_parseServer, ThreeDirectiveOneLine) {
  parser.filecontent_ =
      "server {directive0;"
      "directive1 setting1;"
      "directive2 setting1 setting2;}";
  EXPECT_TRUE(serverOneLine(2, parser.line_count_) ==
              parser.parseServerContext());
}

TEST_F(test_parseServer, ThreeDirectiveMultiLine) {
  parser.filecontent_ =
      "server {\ndirective0;\n"
      "directive1 setting1;\n"
      "directive2 setting1 setting2;\n}";
  EXPECT_TRUE(serverMultiLine(2, parser.line_count_) ==
              parser.parseServerContext());
}

TEST_F(test_parseServer, ThreeDirectiveMultiLineWithComment) {
  parser.filecontent_ =
      "server {\ndirective0; # comment\n"
      "directive1 setting1;# comment\n}"
      "directive2 setting1 setting2;# comment\n# comment\n}";
  EXPECT_TRUE(serverMultiLine(1, parser.line_count_) ==
              parser.parseServerContext());
}

TEST_F(test_parseServer, ThreeDirectiveMultiLineWithOneLineComment) {
  parser.filecontent_ =
      "server {\n"
      "# comment\n"
      "directive0;\n"
      "# comment\n"
      "directive1 setting1;\n"
      "# comment\n"
      "directive2 setting1 setting2;\n}"
      "# comment\n";
  EXPECT_TRUE(serverMultiAndEmptyLine(2, parser.line_count_) ==
              parser.parseServerContext());
}

TEST_F(test_parseServer, ThreeDirectivesAndOneLocationOneLine) {
  parser.filecontent_ =
      "server {"
      "directive0;"
      "location /route/ {"
      "  directive0;"
      "  directive1 setting1;"
      "  directive2 setting1 setting2;"
      "}"
      "directive1 setting1;"
      "directive2 setting1 setting2;}";

  ConfigParser::ServerContextNode expected;
  expected = serverOneLine(2, parser.line_count_);
  expected.locations.push_back(locationOneLine(2, parser.line_count_));

  ConfigParser::ServerContextNode res = parser.parseServerContext();

  EXPECT_TRUE(expected == res);
}

TEST_F(test_parseServer, ThreeDirectivesAndTwoLocationOneLine) {
  parser.filecontent_ =
      "server {"
      "directive0;"
      "location /route/ {"
      "  directive0;"
      "  directive1 setting1;"
      "  directive2 setting1 setting2;"
      "}"
      "location /route/ {"
      "  directive0;"
      "  directive1 setting1;"
      "  directive2 setting1 setting2;"
      "  directive3 setting1 setting2 setting3;"
      "}"
      "directive1 setting1;"
      "directive2 setting1 setting2;}";

  ConfigParser::ServerContextNode expected;
  expected = serverOneLine(2, parser.line_count_);
  expected.locations.push_back(locationOneLine(2, parser.line_count_));
  expected.locations.push_back(locationOneLine(3, parser.line_count_));

  ConfigParser::ServerContextNode res = parser.parseServerContext();

  EXPECT_TRUE(expected == res);
}

TEST_F(test_parseServer, ThreeDirectivesAndTwoLocationMultiLine) {
  parser.filecontent_ =
      "server {\n"
      "directive0;\n"
      "location /route/ {\n"
      "  directive0;\n"
      "  directive1 setting1;\n"
      "  directive2 setting1 setting2;\n"
      "}\n"
      "location /route/ {\n"
      "  directive0;\n"
      "  directive1 setting1;\n"
      "  directive2 setting1 setting2;\n"
      "  directive3 setting1 setting2 setting3;\n"
      "}\n"
      "directive1 setting1;\n"
      "directive2 setting1 setting2;}\n";

  size_t line_no = parser.line_count_;
  ConfigParser::ServerContextNode expected = serverEmpty(line_no++);
  expected.directives.push_back(directive(0, line_no++));
  expected.locations.push_back(locationMultiLine(2, line_no));
  line_no += 5;
  expected.locations.push_back(locationMultiLine(3, line_no));
  line_no += 6;
  expected.directives.push_back(directive(1, line_no++));
  expected.directives.push_back(directive(2, line_no++));

  ConfigParser::ServerContextNode res = parser.parseServerContext();

  EXPECT_TRUE(expected == res);
}

TEST_F(test_parseServer, ThreeDirectivesAndTwoLocationMultiLineComment) {
  parser.filecontent_ =
      "server {\n"
      "#   commmmmmmeeeeennnnnnnntttttt   \n"
      "directive0;\n"
      "#   commmmmmmeeeeennnnnnnntttttt   \n"
      "location /route/ {\n"
      "#   commmmmmmeeeeennnnnnnntttttt   \n"
      "  directive0;\n"
      "#   commmmmmmeeeeennnnnnnntttttt   \n"
      "  directive1 setting1;\n"
      "#   commmmmmmeeeeennnnnnnntttttt   \n"
      "  directive2 setting1 setting2;\n"
      "#   commmmmmmeeeeennnnnnnntttttt   \n"
      "}\n"
      "#   commmmmmmeeeeennnnnnnntttttt   \n"
      "location /route/ {\n"
      "#   commmmmmmeeeeennnnnnnntttttt   \n"
      "  directive0;\n"
      "#   commmmmmmeeeeennnnnnnntttttt   \n"
      "  directive1 setting1;\n"
      "#   commmmmmmeeeeennnnnnnntttttt   \n"
      "  directive2 setting1 setting2;\n"
      "#   commmmmmmeeeeennnnnnnntttttt   \n"
      "  directive3 setting1 setting2 setting3;\n"
      "#   commmmmmmeeeeennnnnnnntttttt   \n"
      "}\n"
      "#   commmmmmmeeeeennnnnnnntttttt   \n"
      "directive1 setting1;\n"
      "#   commmmmmmeeeeennnnnnnntttttt   \n"
      "directive2 setting1 setting2;}\n";

  size_t line_no = parser.line_count_;
  ConfigParser::ServerContextNode expected = serverEmpty(line_no);
  line_no += 2;
  expected.directives.push_back(directive(0, line_no));
  line_no += 2;
  expected.locations.push_back(locationMultiAndEmptyLine(2, line_no));
  line_no += 10;
  expected.locations.push_back(locationMultiAndEmptyLine(3, line_no));
  line_no += 12;
  expected.directives.push_back(directive(1, line_no));
  line_no += 2;
  expected.directives.push_back(directive(2, line_no));

  ConfigParser::ServerContextNode res = parser.parseServerContext();

  EXPECT_TRUE(expected == res);
}

TEST_F(test_parseServer, ErrorNoOpeningBrace) {
  parser.filecontent_ =
      "server \n"
      "directive0;\n"
      "location /route/ {\n"
      "  directive0;\n"
      "  directive1 setting1;\n"
      "  directive2 setting1 setting2;\n"
      "}\n"
      "location /route/ {\n"
      "  directive0;\n"
      "  directive1 setting1;\n"
      "  directive2 setting1 setting2;\n"
      "  directive3 setting1 setting2 setting3;\n"
      "}\n"
      "directive1 setting1;\n"
      "directive2 setting1 setting2;}\n";

  try {
    parser.parseServerContext();
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(),
                 "webserv: error: ConfigParser: line 2: expected '{'");
  }
}

TEST_F(test_parseServer, ErrorNoClosingBrace) {
  parser.filecontent_ =
      "server {\n"
      "directive0;\n"
      "location /route/ {\n"
      "  directive0;\n"
      "  directive1 setting1;\n"
      "  directive2 setting1 setting2;\n"
      "}\n"
      "location /route/ {\n"
      "  directive0;\n"
      "  directive1 setting1;\n"
      "  directive2 setting1 setting2;\n"
      "  directive3 setting1 setting2 setting3;\n"
      "}\n"
      "directive1 setting1;\n"
      "directive2 setting1 setting2;}\n";

  try {
    parser.parseServerContext();
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(),
                 "webserv: error: ConfigParser: line 15: expected '}'");
  }
}

TEST_F(test_parseServer, ErrorNoSemicolon1) {
  parser.filecontent_ =
      "server {\n"
      "directive0\n"
      "location /route/ {\n"
      "  directive0;\n"
      "  directive1 setting1;\n"
      "  directive2 setting1 setting2;\n"
      "}\n"
      "location /route/ {\n"
      "  directive0;\n"
      "  directive1 setting1;\n"
      "  directive2 setting1 setting2;\n"
      "  directive3 setting1 setting2 setting3;\n"
      "}\n"
      "directive1 setting1;\n"
      "directive2 setting1 setting2;}\n";

  try {
    parser.parseServerContext();
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(),
                 "webserv: error: ConfigParser: line 3: expected ';'");
  }
}

TEST_F(test_parseServer, ErrorNoSemicolon2) {
  parser.filecontent_ =
      "server {\n"
      "directive0;\n"
      "location /route/ {\n"
      "  directive0;\n"
      "  directive1 setting1;\n"
      "  directive2 setting1 setting2\n"
      "}\n"
      "location /route/ {\n"
      "  directive0;\n"
      "  directive1 setting1;\n"
      "  directive2 setting1 setting2;\n"
      "  directive3 setting1 setting2 setting3;\n"
      "}\n"
      "directive1 setting1;\n"
      "directive2 setting1 setting2}\n";

  try {
    parser.parseServerContext();
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(),
                 "webserv: error: ConfigParser: line 7: expected ';'");
  }
}

TEST_F(test_parseServer, ErrorNoSemicolon3) {
  parser.filecontent_ =
      "server {\n"
      "directive0;\n"
      "location /route/ {\n"
      "  directive0;\n"
      "  directive1 setting1;\n"
      "  directive2 setting1 setting2;\n"
      "}\n"
      "location /route/ {\n"
      "  directive0;\n"
      "  directive1 setting1;\n"
      "  directive2 setting1 setting2;\n"
      "  directive3 setting1 setting2 setting3;\n"
      "}\n"
      "directive1 setting1;\n"
      "directive2 setting1 setting2;}\n";

  try {
    parser.parseServerContext();
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(),
                 "webserv: error: ConfigParser: line 15: expected ';'");
  }
}

TEST_F(test_parseServer, ErrorUnexpectedServer) {
  parser.filecontent_ =
      "server {\n"
      "directive0;\n"
      "server {\n"
      "  directive0;\n"
      "  directive1 setting1;\n"
      "  directive2 setting1 setting2;\n"
      "}\n"
      "location /route/ {\n"
      "  directive0;\n"
      "  directive1 setting1;\n"
      "  directive2 setting1 setting2;\n"
      "  directive3 setting1 setting2 setting3;\n"
      "}\n"
      "directive1 setting1;\n"
      "directive2 setting1 setting2;}\n";

  try {
    parser.parseServerContext();
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(
        e.what(),
        "webserv: error: ConfigParser: line 3: unexpected server directive");
  }
}
