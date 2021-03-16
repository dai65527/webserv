/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_getNextWord.cc                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/16 10:48:15 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST
#include "ConfigParser.hpp"
#include "gtest.h"

class test_getNextWord : public ::testing::Test {
 protected:
  // 各ケース共通の前処理を書く
  virtual void SetUp() { std::cout << "setup" << std::endl; }

  // 各ケース共通の後処理を書く
  virtual void TearDown() { std::cout << "tear down" << std::endl; }
};

TEST(test_getNextWord, sepByOneSpace) {
  ConfigParser parser;

  parser.char_count_ = 0;
  parser.filecontent_ = "word1 word2 word3 word4 word5";
  std::string res;
  res = parser.getNextWord();
  EXPECT_TRUE(res == "word1");
  res = parser.getNextWord();
  EXPECT_TRUE(res == "word2");
  res = parser.getNextWord();
  EXPECT_TRUE(res == "word3");
  res = parser.getNextWord();
  EXPECT_TRUE(res == "word4");
  res = parser.getNextWord();
  EXPECT_TRUE(res == "word5");
  res = parser.getNextWord();
  EXPECT_TRUE(res == "");
}

TEST(test_getNextWord, sepByManySpace) {
  ConfigParser parser;

  parser.char_count_ = 0;
  parser.filecontent_ =
      "  \n \rword1 \t\v word2\t\tword3 \f\f word4\t\n\v\f\r word5 \r\f";
  std::string res;
  res = parser.getNextWord();
  EXPECT_TRUE(res == "word1");
  res = parser.getNextWord();
  EXPECT_TRUE(res == "word2");
  res = parser.getNextWord();
  EXPECT_TRUE(res == "word3");
  res = parser.getNextWord();
  EXPECT_TRUE(res == "word4");
  res = parser.getNextWord();
  EXPECT_TRUE(res == "word5");
  res = parser.getNextWord();
  EXPECT_TRUE(res == "");
}

TEST(test_getNextWord, sepByComment) {
  ConfigParser parser;

  parser.char_count_ = 0;
  parser.filecontent_ =
      "#this is a comment\nword1 #this is a comment\n word2 word3 # this is another comment    \n "
      "word4###########\nword5#########################";
  std::string res;
  res = parser.getNextWord();
  EXPECT_TRUE(res == "word1");
  res = parser.getNextWord();
  EXPECT_TRUE(res == "word2");
  res = parser.getNextWord();
  EXPECT_TRUE(res == "word3");
  res = parser.getNextWord();
  EXPECT_TRUE(res == "word4");
  res = parser.getNextWord();
  EXPECT_TRUE(res == "word5");
  res = parser.getNextWord();
  EXPECT_TRUE(res == "");
}

TEST(test_getNextWord, justOneWordWithoutSpace) {
  ConfigParser parser;

  parser.char_count_ = 0;
  parser.filecontent_ = "word";
  std::string res;
  res = parser.getNextWord();
  EXPECT_TRUE(res == "word");
  res = parser.getNextWord();
  EXPECT_TRUE(res == "");
}

TEST(test_getNextWord, justOneWordWithSpace) {
  ConfigParser parser;

  parser.char_count_ = 0;
  parser.filecontent_ = "\t\n\v\f\r word\t\n\v\f\r ";
  std::string res;
  res = parser.getNextWord();
  EXPECT_TRUE(res == "word");
  res = parser.getNextWord();
  EXPECT_TRUE(res == "");
}

TEST(test_getNextWord, NoWordNoSpace) {
  ConfigParser parser;

  parser.char_count_ = 0;
  parser.filecontent_ = "\t\n\v\f\r ";
  std::string res;
  res = parser.getNextWord();
  EXPECT_TRUE(res == "");
}

TEST(test_getNextWord, NoWordButSpace) {
  ConfigParser parser;

  parser.char_count_ = 0;
  parser.filecontent_ = "\t\n\v\f\r ";
  std::string res;
  res = parser.getNextWord();
  EXPECT_TRUE(res == "");
}
