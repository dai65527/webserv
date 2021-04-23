/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiParams.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/22 22:31:02 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/04/23 13:59:32 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiParams.hpp"

CgiParams::CgiParams(Session const& session)
    : session_(session), argv_(NULL), envp_(NULL) {}

CgiParams::~CgiParams() {
  if (argv_ != NULL) {
    for (int i = 0; argv_[i] != NULL; ++i) {
      free(argv_[i]);
    }
    free(argv_);
  }
  if (envp_ != NULL) {
    for (int i = 0; envp_[i] != NULL; ++i) {
      free(envp_[i]);
    }
    free(envp_);
  }
}

char** CgiParams::storeArgv(const std::string& filepath,
                            const std::string& cgiuri, const Request& request) {
  std::string argstr;
  /* in case of XXX.cgi?argv1+argv2, if there is "=" in query string, "+"
   * becomes literal not delimitter */
  if (request.getQuery().find("=") == std::string::npos &&
      request.getQuery().find("+") != std::string::npos) {
    argstr = "dummy+" + request.getQuery();
    argv_ = ft_split(argstr.c_str(), '+');
  }
  /* in case of /cgi-bin/XXX.cgi/argv1/argv2 */
  else {
    argstr = "dummy" + session_.getPathInfo(cgiuri);
    argv_ = ft_split(argstr.c_str(), '/');
  }
  if (!argv_) {
    throw std::bad_alloc();
  }
  free(argv_[0]);
  argv_[0] = strdup(filepath.c_str());
  if (!argv_[0]) {
    throw std::bad_alloc();
  }
  return argv_;
}

char** CgiParams::storeMetaVariables(const std::string& cgiuri,
                                     const Request& request) {
  std::vector<std::string> meta_variables;
  std::string tmp;
  const std::map<std::string, std::string>& headers = request.getHeaders();
  tmp = "AUTH_TYPE=";
  tmp += session_.getFromHeaders(headers, "authorization");  // TBC!
  meta_variables.push_back(tmp);
  tmp = "CONTENT_LENGTH=";
  tmp += session_.getFromHeaders(headers, "content-length");
  meta_variables.push_back(tmp);
  tmp = "CONTENT_TYPE=";
  tmp += session_.getFromHeaders(headers, "content-type");
  meta_variables.push_back(tmp);
  tmp = "GATEWAY_INTERFACE=";  //プロトコル名称入れてもらう？
  tmp += session_.getFromHeaders(headers, "gateway-interface");  //?
  meta_variables.push_back(tmp);
  tmp = "PATH_INFO=";  // testerで求める挙動は違うようだ（discord #webserv)
  tmp += session_.getPathInfo(cgiuri);
  meta_variables.push_back(tmp);
  tmp = "PATH_TRANSLATED=";  // Document root + PATH_INFO if there is PATH_INFO
  if (session_.getPathInfo(cgiuri) != "") {
    tmp += session_.findRoot();
    tmp += session_.getPathInfo(cgiuri);
  }
  meta_variables.push_back(tmp);
  tmp = "QUERY_STRING=";
  tmp += request.getQuery();
  meta_variables.push_back(tmp);
  tmp = "REMOTE_ADDR=";
  tmp += getIpAddress(session_.getIp());
  meta_variables.push_back(tmp);
  tmp = "REMOTE_IDENT=";
  tmp += "TEST";  // TBC!
  meta_variables.push_back(tmp);
  tmp = "REMOTE_USER=";
  tmp += "TEST";  // TBC!
  meta_variables.push_back(tmp);
  tmp = "REQUEST_METHOD=";
  tmp += request.getMethod();
  meta_variables.push_back(tmp);
  tmp = "REQUEST_URI=";
  tmp += request.getUri();
  meta_variables.push_back(tmp);
  tmp = "SCRIPT_NAME=";
  tmp += cgiuri;
  meta_variables.push_back(tmp);
  tmp = "SERVER_NAME=";
  tmp += session_.getFromHeaders(headers, "host");
  meta_variables.push_back(tmp);
  tmp = "SERVER_PORT=";
  char* server_port = ft_itoa(ft_htons(session_.getPort()));
  if (!server_port) {
    throw std::bad_alloc();
  }
  tmp += server_port;
  free(server_port);
  meta_variables.push_back(tmp);
  tmp = "SERVER_PROTOCOL=HTTP/1.1";
  meta_variables.push_back(tmp);
  tmp = "SERVER_SOFTWARE=";
  tmp += SOFTWARE_NAME;
  meta_variables.push_back(tmp);
  return vecToChar(meta_variables);
}

char** CgiParams::vecToChar(std::vector<std::string>& meta_variables) {
  envp_ = (char**)malloc(sizeof(char*) * (meta_variables.size() + 1));
  if (!envp_) {
    throw std::bad_alloc();
  }
  for (size_t i = 0; i < meta_variables.size(); ++i) {
    envp_[i] = ft_strdup(meta_variables[i].c_str());
    if (!envp_[i]) {
      throw std::bad_alloc();
    }
  }
  envp_[meta_variables.size()] = NULL;
  return envp_;
}
