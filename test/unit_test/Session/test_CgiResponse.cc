/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_CgiArgvEnvp.cc                               :+:      :+:    :+:   */
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
#include "webserv_settings.hpp"
#include "gtest.h"

class test_CgiResponse : public ::testing::Test {
 protected:
  MainConfig config;
  Session* session;
  bool flg_thrown;
  Session::CgiParams *cgi_params;

  void appendVec(std::vector<char>& vec, const std::string& str) {
    vec.insert(vec.end(), str.begin(), str.end());
  }

  std::string vecToString(std::vector<char>& buf, size_t begin, size_t end) {
    std::string ret;
    for (size_t i = begin; i != end; ++i) {
      ret += buf[i];
    }
    return ret;
  }

  virtual void SetUp() {
    flg_thrown = false;
    session = new Session(0, config);
    session->ip_ = 16777343;  // 127.0.0.1
    session->port_ = 47138;   // 8888
    config.root_ = "./html";
    cgi_params = new Session::CgiParams(*session);
  }

  // 各ケース共通の後処理を書く
  virtual void TearDown() { delete session; }
};

TEST_F(test_CgiResponse, OK1) {
  appendVec(session->request_.buf_,
            "GET /sample.cgi/argv1/argv2/ HTTP/1.1\r\nHost: localhost\r\n\r\n");
  EXPECT_EQ(session->receiveRequest(), 0);
  char** argv = cgi_params->storeArgv("./html/sample.cgi", "/sample.cgi", session->request_);
  char** envp = cgi_params->storeMetaVariables("/sample.cgi", session->request_);
  EXPECT_EQ(HTTP_200, session->cgi_handler_.createCgiProcess("./html/sample.cgi", argv, envp));
  EXPECT_EQ(0, session->readFromCgi());
  EXPECT_EQ("Content-Type: text/hmtl", session->response_.status_header_);
  EXPECT_EQ("<HTML>\n", vecToString(session->response_.body_, 0, 8));
}