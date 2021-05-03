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

#include "CgiParams.hpp"
#include "MainConfig.hpp"
#include "Session.hpp"
#include "gtest.h"

class test_CgiArgvEnvp : public ::testing::Test {
 protected:
  MainConfig config;
  Session* session;
  bool flg_thrown;
  CgiParams* cgi_params;

  void appendVec(std::vector<char>& vec, const std::string& str) {
    vec.insert(vec.end(), str.begin(), str.end());
  }

  virtual void SetUp() {
    flg_thrown = false;
    session = new Session(0, config);
    session->ip_ = 16777343;  // 127.0.0.1
    session->port_ = 47138;   // 8888
    config.root_ = "./html";
    cgi_params = new CgiParams(*session);
  }

  // 各ケース共通の後処理を書く
  virtual void TearDown() { delete session; }
};

TEST_F(test_CgiArgvEnvp, argvOK1) {
  appendVec(session->request_.buf_,
            "GET /sample.cgi/argv1/argv2/ HTTP/1.1\r\nHost: localhost\r\n\r\n");
  EXPECT_EQ(session->receiveRequest(), 0);
  char** argv = cgi_params->storeArgv("./html/sample.cgi", "/sample.cgi",
                                      session->request_);
  EXPECT_EQ("./html/sample.cgi", std::string(argv[0]));
  EXPECT_EQ("argv1", std::string(argv[1]));
  EXPECT_EQ("argv2", std::string(argv[2]));
  EXPECT_EQ(NULL, argv[3]);
}

TEST_F(test_CgiArgvEnvp, argvOK2) {
  appendVec(session->request_.buf_,
            "GET /sample.cgi?argv1+argv2 HTTP/1.1\r\nHost: localhost\r\n\r\n");
  EXPECT_EQ(session->receiveRequest(), 0);
  char** argv = cgi_params->storeArgv("./html/sample.cgi", "/sample.cgi",
                                      session->request_);
  EXPECT_EQ("./html/sample.cgi", std::string(argv[0]));
  EXPECT_EQ("argv1", std::string(argv[1]));
  EXPECT_EQ("argv2", std::string(argv[2]));
  EXPECT_EQ(NULL, argv[3]);
}

TEST_F(test_CgiArgvEnvp, argvOK3) {
  appendVec(session->request_.buf_,
            "GET /cgi-bin/sample.cgi/argv1/argv2 HTTP/1.1\r\nHost: "
            "localhost\r\n\r\n");
  EXPECT_EQ(session->receiveRequest(), 0);
  char** argv = cgi_params->storeArgv("./html/cgi-bin/sample.cgi",
                                      "/cgi-bin/sample.cgi", session->request_);
  EXPECT_EQ("./html/cgi-bin/sample.cgi", std::string(argv[0]));
  EXPECT_EQ("argv1", std::string(argv[1]));
  EXPECT_EQ("argv2", std::string(argv[2]));
  EXPECT_EQ(NULL, argv[3]);
}

TEST_F(test_CgiArgvEnvp, argvOK4) {
  appendVec(session->request_.buf_,
            "GET /cgi-bin/sample.cgi?argv1+argv2 HTTP/1.1\r\nHost: "
            "localhost\r\n\r\n");
  EXPECT_EQ(session->receiveRequest(), 0);
  char** argv = cgi_params->storeArgv("./html/cgi-bin/sample.cgi",
                                      "cgi-bin/sample.cgi", session->request_);
  EXPECT_EQ("./html/cgi-bin/sample.cgi", std::string(argv[0]));
  EXPECT_EQ("argv1", std::string(argv[1]));
  EXPECT_EQ("argv2", std::string(argv[2]));
  EXPECT_EQ(NULL, argv[3]);
}

TEST_F(test_CgiArgvEnvp, NoargvDueToEqual) {
  appendVec(session->request_.buf_,
            "GET /sample.cgi?argv1+argv2=argv3+argv4 HTTP/1.1\r\nHost: "
            "localhost\r\n\r\n");
  EXPECT_EQ(session->receiveRequest(), 0);
  char** argv = cgi_params->storeArgv("./html/sample.cgi", "/sample.cgi",
                                      session->request_);
  EXPECT_EQ("./html/sample.cgi", std::string(argv[0]));
  EXPECT_EQ(NULL, argv[1]);
}

TEST_F(test_CgiArgvEnvp, MetaEnvOK1) {
  appendVec(session->request_.buf_,
            "GET /sample.cgi?argv1+argv2=argv3+argv4 HTTP/1.1\r\nHost: "
            "localhost\r\nAuthorization: Basic abcd:1234\r\n\r\n");
  session->userpass_ = "abcd:1234";
  session->request_.headers_["x-http-header"] = "httpheader";
  session->request_.headers_["Y-Http-Header"] = "http-header";
  session->request_.headers_["Z-HTTP-HEADER"] = "http header";
  EXPECT_EQ(session->receiveRequest(), 0);
  char** envp =
      cgi_params->storeMetaVariables("/sample.cgi", session->request_);
  EXPECT_EQ("AUTH_TYPE=Basic", std::string(*envp++));
  EXPECT_EQ("CONTENT_LENGTH=", std::string(*envp++));
  EXPECT_EQ("CONTENT_TYPE=", std::string(*envp++));
  EXPECT_EQ("GATEWAY_INTERFACE=CGI/1.1", std::string(*envp++));
  EXPECT_EQ("PATH_INFO=/sample.cgi", std::string(*envp++));
  EXPECT_EQ("PATH_TRANSLATED=", std::string(*envp++));
  EXPECT_EQ("QUERY_STRING=argv1+argv2=argv3+argv4", std::string(*envp++));
  EXPECT_EQ("REMOTE_ADDR=127.0.0.1", std::string(*envp++));
  EXPECT_EQ("REMOTE_IDENT=abcd", std::string(*envp++));
  EXPECT_EQ("REMOTE_USER=abcd", std::string(*envp++));
  EXPECT_EQ("REQUEST_METHOD=GET", std::string(*envp++));
  EXPECT_EQ("REQUEST_URI=/sample.cgi", std::string(*envp++));
  EXPECT_EQ("SCRIPT_NAME=/sample.cgi", std::string(*envp++));
  EXPECT_EQ("SERVER_NAME=localhost", std::string(*envp++));
  EXPECT_EQ("SERVER_PORT=8888", std::string(*envp++));
  EXPECT_EQ("SERVER_PROTOCOL=HTTP/1.1", std::string(*envp++));
  EXPECT_EQ(std::string("SERVER_SOFTWARE=") + std::string(SOFTWARE_NAME),
            std::string(*envp++));
  EXPECT_EQ("HTTP_Y_HTTP_HEADER=http-header", std::string(*envp++));
  EXPECT_EQ("HTTP_Z_HTTP_HEADER=http header", std::string(*envp++));
  EXPECT_EQ("HTTP_AUTHORIZATION=Basic abcd:1234", std::string(*envp++));
  EXPECT_EQ("HTTP_HOST=localhost", std::string(*envp++));
  EXPECT_EQ("HTTP_X_HTTP_HEADER=httpheader", std::string(*envp++));
  EXPECT_EQ(NULL, *envp);
}

TEST_F(test_CgiArgvEnvp, MetaEnvOK2) {
  appendVec(session->request_.buf_,
            "POST /sample.cgi/argv1/argv2 HTTP/1.1\r\nHost: "
            "localhost\r\nAuthorization: Basic "
            "user:pass\r\ncontent-length:10\r\n\r\n0123456789\r\n\r\n");
  session->userpass_ = "user:pass";
  session->request_.headers_["x-http-header"] = "httpheader";
  session->request_.headers_["Y-Http-Header"] = "http-header";
  session->request_.headers_["Z-HTTP-HEADER"] = "http header";
  EXPECT_EQ(session->receiveRequest(), 0);
  char** envp =
      cgi_params->storeMetaVariables("/sample.cgi", session->request_);
  EXPECT_EQ("AUTH_TYPE=Basic", std::string(*envp++));
  EXPECT_EQ("CONTENT_LENGTH=10", std::string(*envp++));
  EXPECT_EQ("CONTENT_TYPE=", std::string(*envp++));
  EXPECT_EQ("GATEWAY_INTERFACE=CGI/1.1", std::string(*envp++));
  EXPECT_EQ("PATH_INFO=/sample.cgi", std::string(*envp++));
  EXPECT_EQ("PATH_TRANSLATED=./html/argv1/argv2", std::string(*envp++));
  EXPECT_EQ("QUERY_STRING=", std::string(*envp++));
  EXPECT_EQ("REMOTE_ADDR=127.0.0.1", std::string(*envp++));
  EXPECT_EQ("REMOTE_IDENT=user", std::string(*envp++));
  EXPECT_EQ("REMOTE_USER=user", std::string(*envp++));
  EXPECT_EQ("REQUEST_METHOD=POST", std::string(*envp++));
  EXPECT_EQ("REQUEST_URI=/sample.cgi/argv1/argv2", std::string(*envp++));
  EXPECT_EQ("SCRIPT_NAME=/sample.cgi", std::string(*envp++));
  EXPECT_EQ("SERVER_NAME=localhost", std::string(*envp++));
  EXPECT_EQ("SERVER_PORT=8888", std::string(*envp++));
  EXPECT_EQ("SERVER_PROTOCOL=HTTP/1.1", std::string(*envp++));
  EXPECT_EQ(std::string("SERVER_SOFTWARE=") + std::string(SOFTWARE_NAME),
            std::string(*envp++));
  EXPECT_EQ("HTTP_Y_HTTP_HEADER=http-header", std::string(*envp++));
  EXPECT_EQ("HTTP_Z_HTTP_HEADER=http header", std::string(*envp++));
  EXPECT_EQ("HTTP_AUTHORIZATION=Basic user:pass", std::string(*envp++));
  EXPECT_EQ("HTTP_CONTENT_LENGTH=10", std::string(*envp++));
  EXPECT_EQ("HTTP_HOST=localhost", std::string(*envp++));
  EXPECT_EQ("HTTP_X_HTTP_HEADER=httpheader", std::string(*envp++));
  EXPECT_EQ(NULL, *envp);
}
