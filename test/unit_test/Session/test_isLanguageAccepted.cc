/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_isLanguageAccepted.cc                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/04/23 09:17:24 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "MainConfig.hpp"
#include "Session.hpp"
#include "gtest.h"

class test_isLanguageAccepted : public ::testing::Test {
 protected:
  MainConfig config;
  Session* session;
virtual void SetUp() {
    session = new Session(0, config);
    session->request_.headers_["host"] = "localhost:8080";
  }

  // 各ケース共通の後処理を書く
  virtual void TearDown() { delete session; }
};

TEST_F(test_isLanguageAccepted, only_ja_vs_ja) {
  config.language_.push_back("ja");
  session->request_.headers_["accept-language"] = "ja";
  EXPECT_TRUE(session->isLanguageAccepted());
}

TEST_F(test_isLanguageAccepted, only_ja_vs_ja_en_es) {
  config.language_.push_back("ja");
  session->request_.headers_["accept-language"] = "ja, en, es";
  EXPECT_TRUE(session->isLanguageAccepted());
}

TEST_F(test_isLanguageAccepted, only_en_vs_ja_en_es) {
  config.language_.push_back("en");
  session->request_.headers_["accept-language"] = "ja, en, es";
  EXPECT_TRUE(session->isLanguageAccepted());
}

TEST_F(test_isLanguageAccepted, only_ja_vs_ja_JP) {
  config.language_.push_back("ja");
  session->request_.headers_["accept-language"] = "ja-JP";
  EXPECT_TRUE(session->isLanguageAccepted());
}

TEST_F(test_isLanguageAccepted, only_ja_vs_ja_JP_en_US_es_US) {
  config.language_.push_back("ja");
  session->request_.headers_["accept-language"] = "ja-JP, en-US, es-US";
  EXPECT_TRUE(session->isLanguageAccepted());
}

TEST_F(test_isLanguageAccepted, only_en_vs_ja_JP_en_US_es_US) {
  config.language_.push_back("en");
  session->request_.headers_["accept-language"] = "ja-JP  , en-US, es-US";
  EXPECT_TRUE(session->isLanguageAccepted());
}

TEST_F(test_isLanguageAccepted, only_es_vs_ja_JP_en_US_es_US) {
  config.language_.push_back("es");
  session->request_.headers_["accept-language"] = "ja-JP  , en-US , es-US";
  EXPECT_TRUE(session->isLanguageAccepted());
}

TEST_F(test_isLanguageAccepted, only_ja_vs_ja_JP_en_US_es_US_with_q) {
  config.language_.push_back("ja");
  session->request_.headers_["accept-language"] =
      "ja-JP;q=0.8, en-US;q=0.8,es-US;q=0.1";
  EXPECT_TRUE(session->isLanguageAccepted());
}

TEST_F(test_isLanguageAccepted, only_en_vs_ja_JP_en_US_es_US_with_q) {
  config.language_.push_back("en");
  session->request_.headers_["accept-language"] =
      "ja-JP;q=0.8, en-US;q=0.8,es-US;q=0.1";
  EXPECT_TRUE(session->isLanguageAccepted());
}

TEST_F(test_isLanguageAccepted, only_es_vs_ja_JP_en_US_es_US_with_q) {
  config.language_.push_back("es");
  session->request_.headers_["accept-language"] =
      "ja-JP;q=0.8  , en-US  ;q=0.8  , es-US;q=0.1";
  EXPECT_TRUE(session->isLanguageAccepted());
}

///////


TEST_F(test_isLanguageAccepted, only_ja_JP_vs_ja) {
  config.language_.push_back("ja-JP");
  session->request_.headers_["accept-language"] = "ja";
  EXPECT_TRUE(session->isLanguageAccepted());
}

TEST_F(test_isLanguageAccepted, only_ja_JP_vs_ja_en_es) {
  config.language_.push_back("ja-JP");
  session->request_.headers_["accept-language"] = "ja, en, es";
  EXPECT_TRUE(session->isLanguageAccepted());
}

TEST_F(test_isLanguageAccepted, only_en_US_vs_ja_en_es) {
  config.language_.push_back("en-US");
  session->request_.headers_["accept-language"] = "ja, en, es";
  EXPECT_TRUE(session->isLanguageAccepted());
}

TEST_F(test_isLanguageAccepted, only_es_vs_ja_en_es) {
  config.language_.push_back("es-US");
  session->request_.headers_["accept-language"] = "ja, en, es";
  EXPECT_TRUE(session->isLanguageAccepted());
}

TEST_F(test_isLanguageAccepted, only_ja_JP_vs_ja_JP) {
  config.language_.push_back("ja-JP");
  session->request_.headers_["accept-language"] = "ja-JP";
  EXPECT_TRUE(session->isLanguageAccepted());
}

TEST_F(test_isLanguageAccepted, only_ja_JP_vs_ja_JP_en_US_es_US) {
  config.language_.push_back("ja-JP");
  session->request_.headers_["accept-language"] = "ja-JP, en-US, es-US";
  EXPECT_TRUE(session->isLanguageAccepted());
}

TEST_F(test_isLanguageAccepted, only_en_US_vs_ja_JP_en_US_es_US) {
  config.language_.push_back("en-US");
  session->request_.headers_["accept-language"] = "ja-JP, en-US, es-US";
  EXPECT_TRUE(session->isLanguageAccepted());
}

TEST_F(test_isLanguageAccepted, only_es_US_vs_ja_JP_en_US_es_US) {
  config.language_.push_back("es-US");
  session->request_.headers_["accept-language"] = "ja-JP, en-US, es-US";
  EXPECT_TRUE(session->isLanguageAccepted());
}

TEST_F(test_isLanguageAccepted, only_ja_JP_vs_ja_JP_en_US_es_US_with_q) {
  config.language_.push_back("ja-JP");
  session->request_.headers_["accept-language"] =
      "ja-JP;q=0.8, en-US;q=0.8,es-US;q=0.1";
  EXPECT_TRUE(session->isLanguageAccepted());
}

TEST_F(test_isLanguageAccepted, only_en_US_vs_ja_JP_en_US_es_US_with_q) {
  config.language_.push_back("en-US");
  session->request_.headers_["accept-language"] =
      "ja-JP;q=0.8, en-US;q=0.8,es-US;q=0.1";
  EXPECT_TRUE(session->isLanguageAccepted());
}

TEST_F(test_isLanguageAccepted, only_es_US_vs_ja_JP_en_US_es_US_with_q) {
  config.language_.push_back("es-US");
  session->request_.headers_["accept-language"] =
      "ja-JP;q=0.8, en-US;q=0.8,es-US;q=0.1";
  EXPECT_TRUE(session->isLanguageAccepted());
}

/// case won't match

TEST_F(test_isLanguageAccepted, only_ko_vs_ja) {
  config.language_.push_back("ko");
  session->request_.headers_["accept-language"] = "ja";
  EXPECT_FALSE(session->isLanguageAccepted());
}

TEST_F(test_isLanguageAccepted, only_ko_vs_ja_en_es) {
  config.language_.push_back("ko");
  session->request_.headers_["accept-language"] = "ja, en, es";
  EXPECT_FALSE(session->isLanguageAccepted());
}

TEST_F(test_isLanguageAccepted, only_ja_US_vs_ja_JP) {
  config.language_.push_back("ja-US");
  session->request_.headers_["accept-language"] = "ja-JP";
  EXPECT_FALSE(session->isLanguageAccepted());
}

TEST_F(test_isLanguageAccepted, only_en_JP_vs_ja_JP) {
  config.language_.push_back("en-JP");
  session->request_.headers_["accept-language"] = "ja-JP";
  EXPECT_FALSE(session->isLanguageAccepted());
}

TEST_F(test_isLanguageAccepted, only_ja_US_vs_ja_JP_en_US_es_US) {
  config.language_.push_back("ja-US");
  session->request_.headers_["accept-language"] = "ja-JP, en-US, es-US";
  EXPECT_FALSE(session->isLanguageAccepted());
}

TEST_F(test_isLanguageAccepted, only_en_JP_vs_ja_JP_en_US_es_US) {
  config.language_.push_back("en-JP");
  session->request_.headers_["accept-language"] = "ja-JP, en-US, es-US";
  EXPECT_FALSE(session->isLanguageAccepted());
}

TEST_F(test_isLanguageAccepted, only_ja_US_vs_ja_JP_en_US_es_US_with_q) {
  config.language_.push_back("ja-US");
  session->request_.headers_["accept-language"] =
      "ja-JP;q=0.8, en-US;q=0.8,es-US;q=0.1";
  EXPECT_FALSE(session->isLanguageAccepted());
}

TEST_F(test_isLanguageAccepted, only_en_JP_vs_ja_JP_en_US_es_US_with_q) {
  config.language_.push_back("en-JP");
  session->request_.headers_["accept-language"] =
      "ja-JP;q=0.8, en-US;q=0.8,es-US;q=0.1";
  EXPECT_FALSE(session->isLanguageAccepted());
}

/// case won't match will match with *

TEST_F(test_isLanguageAccepted, only_ko_vs_ja_wild) {
  config.language_.push_back("ko");
  session->request_.headers_["accept-language"] = "ja, *";
  EXPECT_TRUE(session->isLanguageAccepted());
}

TEST_F(test_isLanguageAccepted, only_ko_vs_ja_en_es_wild) {
  config.language_.push_back("ko");
  session->request_.headers_["accept-language"] = "ja, en, es, *";
  EXPECT_TRUE(session->isLanguageAccepted());
}

TEST_F(test_isLanguageAccepted, only_ja_US_vs_ja_JP_wild) {
  config.language_.push_back("ja-US");
  session->request_.headers_["accept-language"] = "ja-JP, *";
  EXPECT_TRUE(session->isLanguageAccepted());
}

TEST_F(test_isLanguageAccepted, only_en_JP_vs_ja_JP_wild) {
  config.language_.push_back("en-JP");
  session->request_.headers_["accept-language"] = "ja-JP, *";
  EXPECT_TRUE(session->isLanguageAccepted());
}

TEST_F(test_isLanguageAccepted, only_ja_US_vs_ja_JP_en_US_es_US_wild) {
  config.language_.push_back("ja-US");
  session->request_.headers_["accept-language"] = "ja-JP, en-US, es-US, *";
  EXPECT_TRUE(session->isLanguageAccepted());
}

TEST_F(test_isLanguageAccepted, only_en_JP_vs_ja_JP_en_US_es_US_wild) {
  config.language_.push_back("en-JP");
  session->request_.headers_["accept-language"] = "ja-JP, en-US, es-US, *";
  EXPECT_TRUE(session->isLanguageAccepted());
}

TEST_F(test_isLanguageAccepted, only_ja_US_vs_ja_JP_en_US_es_US_wild_with_q) {
  config.language_.push_back("ja-US");
  session->request_.headers_["accept-language"] =
      "ja-JP;q=0.8, en-US;q=0.8,es-US;q=0.1, *";
  EXPECT_TRUE(session->isLanguageAccepted());
}

TEST_F(test_isLanguageAccepted, only_en_JP_vs_ja_JP_en_US_es_US_wild_with_q) {
  config.language_.push_back("en-JP");
  session->request_.headers_["accept-language"] =
      "ja-JP;q=0.8, en-US;q=0.8,es-US;q=0.1, *";
  EXPECT_TRUE(session->isLanguageAccepted());
}

// TEST_F(test_isLanguageAccepted, onlyOneBig) {
//   config.charset_ = "UTF-8";
//   session->request_.headers_["accept-language"] = "UTF-8";
//   EXPECT_TRUE(session->isLanguageAccepted("text/html"));
// }

// TEST_F(test_isLanguageAccepted, onlyOneSmallBig) {
//   config.charset_ = "UTF-8";
//   session->request_.headers_["accept-language"] = "utf-8";
//   EXPECT_TRUE(session->isLanguageAccepted("text/html"));
// }

// TEST_F(test_isLanguageAccepted, onlyOneBigSmall) {
//   config.charset_ = "utf-8";
//   session->request_.headers_["accept-language"] = "UTF-8";
//   EXPECT_TRUE(session->isLanguageAccepted("text/html"));
// }

// TEST_F(test_isLanguageAccepted, onlyOneMixed) {
//   config.charset_ = "UtF-8";
//   session->request_.headers_["accept-language"] = "uTf-8";
//   EXPECT_TRUE(session->isLanguageAccepted("text/html"));
// }

// TEST_F(test_isLanguageAccepted, onlyOneDiff1) {
//   config.charset_ = "utf-8";
//   session->request_.headers_["accept-language"] = "utf-";
//   EXPECT_FALSE(session->isLanguageAccepted("text/html"));
// }

// TEST_F(test_isLanguageAccepted, onlyOneDiff2) {
//   config.charset_ = "utf-8";
//   session->request_.headers_["accept-language"] = "utf-82";
//   EXPECT_FALSE(session->isLanguageAccepted("text/html"));
// }

// TEST_F(test_isLanguageAccepted, onlyOneDiff3) {
//   config.charset_ = "utf-8";
//   session->request_.headers_["accept-language"] = "utf-9";
//   EXPECT_FALSE(session->isLanguageAccepted("text/html"));
// }

// TEST_F(test_isLanguageAccepted, onlyOneDiff4) {
//   config.charset_ = "utf-";
//   session->request_.headers_["accept-language"] = "utf-8";
//   EXPECT_FALSE(session->isLanguageAccepted("text/html"));
// }

// TEST_F(test_isLanguageAccepted, onlyOneDiff5) {
//   config.charset_ = "utf-9";
//   session->request_.headers_["accept-language"] = "utf-8";
//   EXPECT_FALSE(session->isLanguageAccepted("text/html"));
// }

// TEST_F(test_isLanguageAccepted, onlyOneDiff6) {
//   config.charset_ = "utf-82";
//   session->request_.headers_["accept-language"] = "utf-8";
//   EXPECT_FALSE(session->isLanguageAccepted("text/html"));
// }

// TEST_F(test_isLanguageAccepted, onlyOneWild) {
//   config.charset_ = "utf-8";
//   session->request_.headers_["accept-language"] = "*";
//   EXPECT_TRUE(session->isLanguageAccepted("text/html"));
// }

// TEST_F(test_isLanguageAccepted, multiTwoFirst) {
//   config.charset_ = "utf-8";
//   session->request_.headers_["accept-language"] = "utf-8, iso-8859-1;q=0.5";
//   EXPECT_TRUE(session->isLanguageAccepted("text/html"));
// }

// TEST_F(test_isLanguageAccepted, multiTwoSecond) {
//   config.charset_ = "iso-8859-1";
//   session->request_.headers_["accept-language"] = "utf-8, iso-8859-1;q=0.5";
//   EXPECT_TRUE(session->isLanguageAccepted("text/html"));
// }

// TEST_F(test_isLanguageAccepted, multiTwoWildCardFirst) {
//   config.charset_ = "utf-8";
//   session->request_.headers_["accept-language"] =
//       "utf-8, iso-8859-1;q=0.5, *;q=0.1";
//   EXPECT_TRUE(session->isLanguageAccepted("text/html"));
// }

// TEST_F(test_isLanguageAccepted, multiTwoWildCardSecond) {
//   config.charset_ = "iso-8859-1";
//   session->request_.headers_["accept-language"] =
//       "utf-8, iso-8859-1;q=0.5, *;q=0.1";
//   EXPECT_TRUE(session->isLanguageAccepted("text/html"));
// }

// TEST_F(test_isLanguageAccepted, multiTwoWildCardWild) {
//   config.charset_ = "shift-jis";
//   session->request_.headers_["accept-language"] =
//       "utf-8, iso-8859-1;q=0.5, *;q=0.1";
//   EXPECT_TRUE(session->isLanguageAccepted("text/html"));
// }

// TEST_F(test_isLanguageAccepted, multiTwoWildCardShouldNotMatch1) {
//   config.charset_ = "q=0.5";
//   session->request_.headers_["accept-language"] = "utf-8, iso-8859-1;q=0.5";
//   EXPECT_FALSE(session->isLanguageAccepted("text/html"));
// }

// TEST_F(test_isLanguageAccepted, multiTwoWildCardShouldNotMatchJson) {
//   config.charset_ = "q=0.5";
//   session->request_.headers_["accept-language"] = "utf-8, iso-8859-1;q=0.5";
//   EXPECT_FALSE(session->isLanguageAccepted("application/json"));
// }

// TEST_F(test_isLanguageAccepted, multiTwoWildCardShouldNotMatch1ButNotText) {
//   config.charset_ = "q=0.5";
//   session->request_.headers_["accept-language"] = "utf-8, iso-8859-1;q=0.5";
//   EXPECT_TRUE(session->isLanguageAccepted("application/octed-stream"));
// }19
