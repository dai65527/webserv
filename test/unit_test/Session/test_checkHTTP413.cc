/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_checkHTTP413.cc                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:22:26 by dnakano           #+#    #+#             */
/*   Updated: 2021/04/02 14:21:46 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "MainConfig.hpp"
#include "Session.hpp"
#include "gtest.h"

class test_checkHTTP413 : public ::testing::Test {
 protected:
  MainConfig config;
  Session *session;
  bool flg_thrown;

  void appendVec(std::vector<char> &vec, const std::string &str) {
    vec.insert(vec.end(), str.begin(), str.end());
  }

  virtual void SetUp() {
    flg_thrown = false;
    session = new Session(0, config);
  }

  // 各ケース共通の後処理を書く
  virtual void TearDown() { delete session; }
};

TEST_F(test_checkHTTP413, mainConfigOK) {
  config.client_max_body_size_ = 42;
  appendVec(session->request_.buf_, "POST /index.html?a HTTP/1.1\r\nHost: ");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "ama\r\nlanguage: \t\t");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "en-US\r\nContent-length: \t\t");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "16\r\n\r\n012345678");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "9abcdefghij");
  EXPECT_EQ(session->request_.method_, "POST");
  EXPECT_EQ(session->request_.uri_, "/index.html");
  EXPECT_EQ(session->request_.query_["a"], "");
  EXPECT_EQ(session->request_.headers_["host"], "localhost");
  EXPECT_EQ(session->request_.headers_["location"], "Yokohama");
  EXPECT_EQ(session->request_.headers_["language"], "en-US");
  EXPECT_EQ(session->request_.headers_["content-length"], "16");
  EXPECT_EQ(session->receiveRequest(), 0);
  // std::string str(session->request_.buf_.begin(), session->request_.buf_.end());
  std::string str(session->request_.body_.begin(), session->request_.body_.end());
  EXPECT_EQ(str, "0123456789abcdef");
}

TEST_F(test_checkHTTP413, mainConfigNG) {
  config.client_max_body_size_ = 42;
  appendVec(session->request_.buf_, "POST /index.html?a HTTP/1.1\r\nHost: ");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "ama\r\nlanguage: \t\t");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "en-US\r\nContent-length: \t\t");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "100\r\n\r\n012345678");
  EXPECT_EQ(session->receiveRequest(),
            4133);  // max body size defined in main config
  EXPECT_EQ(session->request_.method_, "POST");
  EXPECT_EQ(session->request_.uri_, "/index.html");
  EXPECT_EQ(session->request_.query_["a"], "");
  EXPECT_EQ(session->request_.headers_["host"], "localhost");
  EXPECT_EQ(session->request_.headers_["location"], "Yokohama");
  EXPECT_EQ(session->request_.headers_["language"], "en-US");
  EXPECT_EQ(session->request_.headers_["content-length"], "100");
}

TEST_F(test_checkHTTP413, serverConfigOK) {
  config.client_max_body_size_ = 42;
  ServerConfig server_config;
  server_config.client_max_body_size_ = 30;
  session->server_config_ = &server_config;
  appendVec(session->request_.buf_, "POST /index.html?a HTTP/1.1\r\nHost: ");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "ama\r\nlanguage: \t\t");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "en-US\r\nContent-length: \t\t");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "16\r\n\r\n012345678");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "9abcdefghij");
  EXPECT_EQ(session->request_.method_, "POST");
  EXPECT_EQ(session->request_.uri_, "/index.html");
  EXPECT_EQ(session->request_.query_["a"], "");
  EXPECT_EQ(session->request_.headers_["host"], "localhost");
  EXPECT_EQ(session->request_.headers_["location"], "Yokohama");
  EXPECT_EQ(session->request_.headers_["language"], "en-US");
  EXPECT_EQ(session->request_.headers_["content-length"], "16");
  EXPECT_EQ(session->receiveRequest(), 0);
  // std::string str(session->request_.buf_.begin(), session->request_.buf_.end());
  std::string str(session->request_.body_.begin(), session->request_.body_.end());
  EXPECT_EQ(str, "0123456789abcdef");
}

TEST_F(test_checkHTTP413, serverConfigNG) {
  config.client_max_body_size_ = 42;
  ServerConfig server_config;
  server_config.client_max_body_size_ = 30;
  session->server_config_ = &server_config;
  appendVec(session->request_.buf_, "POST /index.html?a HTTP/1.1\r\nHost: ");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "ama\r\nlanguage: \t\t");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "en-US\r\nContent-length: \t\t");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "40\r\n\r\n012345678");
  EXPECT_EQ(session->receiveRequest(),
            4132);  // max body size defined in server config
  EXPECT_EQ(session->request_.method_, "POST");
  EXPECT_EQ(session->request_.uri_, "/index.html");
  EXPECT_EQ(session->request_.query_["a"], "");
  EXPECT_EQ(session->request_.headers_["host"], "localhost");
  EXPECT_EQ(session->request_.headers_["location"], "Yokohama");
  EXPECT_EQ(session->request_.headers_["language"], "en-US");
  EXPECT_EQ(session->request_.headers_["content-length"], "40");
}

TEST_F(test_checkHTTP413, locationConfigOK) {
  config.client_max_body_size_ = 42;
  ServerConfig server_config;
  server_config.client_max_body_size_ = 30;
  session->server_config_ = &server_config;
  LocationConfig location_config;
  location_config.client_max_body_size_ = 20;
  session->location_config_ = &location_config;
  appendVec(session->request_.buf_, "POST /index.html?a HTTP/1.1\r\nHost: ");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "ama\r\nlanguage: \t\t");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "en-US\r\nContent-length: \t\t");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "16\r\n\r\n012345678");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "9abcdefghij");
  EXPECT_EQ(session->request_.method_, "POST");
  EXPECT_EQ(session->request_.uri_, "/index.html");
  EXPECT_EQ(session->request_.query_["a"], "");
  EXPECT_EQ(session->request_.headers_["host"], "localhost");
  EXPECT_EQ(session->request_.headers_["location"], "Yokohama");
  EXPECT_EQ(session->request_.headers_["language"], "en-US");
  EXPECT_EQ(session->request_.headers_["content-length"], "16");
  EXPECT_EQ(session->receiveRequest(), 0);
  // std::string str(session->request_.buf_.begin(), session->request_.buf_.end());
  std::string str(session->request_.body_.begin(), session->request_.body_.end());
  EXPECT_EQ(str, "0123456789abcdef");
}

TEST_F(test_checkHTTP413, locationConfigNG) {
  config.client_max_body_size_ = 42;
  ServerConfig server_config;
  server_config.client_max_body_size_ = 30;
  session->server_config_ = &server_config;
  LocationConfig location_config;
  location_config.client_max_body_size_ = 20;
  session->location_config_ = &location_config;
  appendVec(session->request_.buf_, "POST /index.html?a HTTP/1.1\r\nHost: ");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "ama\r\nlanguage: \t\t");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "en-US\r\nContent-length: \t\t");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "30\r\n\r\n012345678");
  EXPECT_EQ(session->receiveRequest(),
            4131);  // max body size defined in location config
  EXPECT_EQ(session->request_.method_, "POST");
  EXPECT_EQ(session->request_.uri_, "/index.html");
  EXPECT_EQ(session->request_.query_["a"], "");
  EXPECT_EQ(session->request_.headers_["host"], "localhost");
  EXPECT_EQ(session->request_.headers_["location"], "Yokohama");
  EXPECT_EQ(session->request_.headers_["language"], "en-US");
  EXPECT_EQ(session->request_.headers_["content-length"], "30");
}

TEST_F(test_checkHTTP413, transferEncodingOK1) {
  config.client_max_body_size_ = 42;
  appendVec(session->request_.buf_, "POST /index.html?a HTTP/1.1\r\nHost: ");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "ama\r\nlanguage: \t\t");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "en-US\r\nTransfer-Encoding: \t\t");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "Chunked\r\n\r\n012345678");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "9abcdefghij");
  EXPECT_EQ(session->request_.method_, "POST");
  EXPECT_EQ(session->request_.uri_, "/index.html");
  EXPECT_EQ(session->request_.query_["a"], "");
  EXPECT_EQ(session->request_.headers_["host"], "localhost");
  EXPECT_EQ(session->request_.headers_["location"], "Yokohama");
  EXPECT_EQ(session->request_.headers_["language"], "en-US");
  EXPECT_EQ(session->request_.headers_["transfer-encoding"], "chunked");
}

TEST_F(test_checkHTTP413, transferEncodingNG1) {
  config.client_max_body_size_ = 42;
  appendVec(session->request_.buf_, "POST /index.html?a HTTP/1.1\r\nHost: ");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "ama\r\nlanguage: \t\t");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "en-US\r\nTransfer-Encoding: \t\t");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "hunked\r\n\r\n012345678");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "9abcdefghij");
  EXPECT_EQ(session->request_.method_, "POST");
  EXPECT_EQ(session->request_.uri_, "/index.html");
  EXPECT_EQ(session->request_.query_["a"], "");
  EXPECT_EQ(session->request_.headers_["host"], "localhost");
  EXPECT_EQ(session->request_.headers_["location"], "Yokohama");
  EXPECT_EQ(session->request_.headers_["language"], "en-US");
  EXPECT_EQ(session->request_.headers_["transfer-encoding"], "hunked");
}

TEST_F(test_checkHTTP413, transferEncodingNG2) {
  config.client_max_body_size_ = 42;
  appendVec(session->request_.buf_, "POST /index.html?a HTTP/1.1\r\nHost: ");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "ama\r\nlanguage: \t\t");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "en-US\r\nTransfer-Encoding: \t\t");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "chunked\r\n\r\nZZZ\r\n");
  EXPECT_EQ(session->receiveRequest(), 0);
  EXPECT_EQ(session->request_.method_, "POST");
  EXPECT_EQ(session->request_.uri_, "/index.html");
  EXPECT_EQ(session->request_.query_["a"], "");
  EXPECT_EQ(session->request_.headers_["host"], "localhost");
  EXPECT_EQ(session->request_.headers_["location"], "Yokohama");
  EXPECT_EQ(session->request_.headers_["language"], "en-US");
  EXPECT_EQ(session->request_.headers_["transfer-encoding"], "chunked");
  EXPECT_EQ(session->receiveRequest(), 0);
}

TEST_F(test_checkHTTP413, transferEncodingOK2) {
  config.client_max_body_size_ = 42;
  appendVec(session->request_.buf_, "POST /index.html?a HTTP/1.1\r\nHost: ");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "ama\r\nlanguage: \t\t");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "en-US\r\nTransfer-Encoding: \t\t");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "chunked\r\n\r\n5\r\n01234\r\n0\r\n\r\n");
  EXPECT_EQ(session->receiveRequest(), 0);
  EXPECT_EQ(session->request_.method_, "POST");
  EXPECT_EQ(session->request_.uri_, "/index.html");
  EXPECT_EQ(session->request_.query_["a"], "");
  EXPECT_EQ(session->request_.headers_["host"], "localhost");
  EXPECT_EQ(session->request_.headers_["location"], "Yokohama");
  EXPECT_EQ(session->request_.headers_["language"], "en-US");
  EXPECT_EQ(session->request_.headers_["transfer-encoding"], "chunked");
  EXPECT_EQ(session->receiveRequest(), 0);
  EXPECT_EQ(session->receiveRequest(), 0);
  EXPECT_EQ(session->receiveRequest(), 0);
  EXPECT_EQ(session->receiveRequest(), 0);
  std::string str(session->request_.body_.begin(), session->request_.body_.end());
  EXPECT_EQ(str, "01234");
}

TEST_F(test_checkHTTP413, transferEncodingOK3) {
  config.client_max_body_size_ = 42;
  appendVec(session->request_.buf_, "POST /index.html?a HTTP/1.1\r\nHost: ");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "ama\r\nlanguage: \t\t");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "en-US\r\nTransfer-Encoding: \t\t");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "chunked\r\n\r\n5\r\n01234\r\n3");
  EXPECT_EQ(session->receiveRequest(), 0);
  EXPECT_EQ(session->request_.method_, "POST");
  EXPECT_EQ(session->request_.uri_, "/index.html");
  EXPECT_EQ(session->request_.query_["a"], "");
  EXPECT_EQ(session->request_.headers_["host"], "localhost");
  EXPECT_EQ(session->request_.headers_["location"], "Yokohama");
  EXPECT_EQ(session->request_.headers_["language"], "en-US");
  EXPECT_EQ(session->request_.headers_["transfer-encoding"], "chunked");
  EXPECT_EQ(session->receiveRequest(), 0);
  EXPECT_EQ(session->receiveRequest(), 0);
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "\r\n567\r\n0\r\n\r\n");
  EXPECT_EQ(session->receiveRequest(), 0);
  EXPECT_EQ(session->receiveRequest(), 0);
  EXPECT_EQ(session->receiveRequest(), 0);
  EXPECT_EQ(session->receiveRequest(), 0);
  std::string str(session->request_.body_.begin(), session->request_.body_.end());
  EXPECT_EQ(str, "01234567");
}

TEST_F(test_checkHTTP413, transferEncodingNG3) {
  config.client_max_body_size_ = 42;
  appendVec(session->request_.buf_, "POST /index.html?a HTTP/1.1\r\nHost: ");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "ama\r\nlanguage: \t\t");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "en-US\r\nTransfer-Encoding: \t\t");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "chunked\r\n\r\n5\r\n01234\r\nA");
  EXPECT_EQ(session->receiveRequest(), 0);
  EXPECT_EQ(session->request_.method_, "POST");
  EXPECT_EQ(session->request_.uri_, "/index.html");
  EXPECT_EQ(session->request_.query_["a"], "");
  EXPECT_EQ(session->request_.headers_["host"], "localhost");
  EXPECT_EQ(session->request_.headers_["location"], "Yokohama");
  EXPECT_EQ(session->request_.headers_["language"], "en-US");
  EXPECT_EQ(session->request_.headers_["transfer-encoding"], "chunked");
  EXPECT_EQ(session->receiveRequest(), 0);
  EXPECT_EQ(session->receiveRequest(), 0);
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "\r\n0123456789ab\r\n");
  EXPECT_EQ(session->receiveRequest(), 0);
  EXPECT_EQ(session->receiveRequest(), 0);
  std::string str(session->request_.body_.begin(), session->request_.body_.end());
  EXPECT_EQ(str, "01234");
}

TEST_F(test_checkHTTP413, transferEncodingOK4) {
  config.client_max_body_size_ = 42;
  appendVec(session->request_.buf_, "POST /index.html?a HTTP/1.1\r\nHost: ");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "localhost\r\nLocation:\t  \tYokoh");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "ama\r\nlanguage: \t\t");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "en-US\r\nTransfer-Encoding: \t\t");
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "chunked\r\n\r\n5\r\n01234\r\n3");
  EXPECT_EQ(session->receiveRequest(), 0);
  EXPECT_EQ(session->request_.method_, "POST");
  EXPECT_EQ(session->request_.uri_, "/index.html");
  EXPECT_EQ(session->request_.query_["a"], "");
  EXPECT_EQ(session->request_.headers_["host"], "localhost");
  EXPECT_EQ(session->request_.headers_["location"], "Yokohama");
  EXPECT_EQ(session->request_.headers_["language"], "en-US");
  EXPECT_EQ(session->request_.headers_["transfer-encoding"], "chunked");
  EXPECT_EQ(session->receiveRequest(), 0);
  EXPECT_EQ(session->receiveRequest(), 0);
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "\r\n567\r\nFF\r\n123456789abcdef");
  EXPECT_EQ(session->receiveRequest(), 0);
  EXPECT_EQ(session->receiveRequest(), 0);
  EXPECT_EQ(session->receiveRequest(), 0);
  EXPECT_EQ(session->receiveRequest(), 0);
  appendVec(session->request_.buf_, "\r\n0\r\n\r\n");
  EXPECT_EQ(session->receiveRequest(), 0);
  EXPECT_EQ(session->receiveRequest(), 0);
  EXPECT_EQ(session->receiveRequest(), 0);
  std::string str(session->request_.body_.begin(), session->request_.body_.end());
  EXPECT_EQ(str, "01234567123456789abcdef");
}