/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/02 01:32:00 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/04/11 21:18:22 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SESSION_HPP
#define SESSION_HPP

#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "CgiHandler.hpp"
#include "LocationConfig.hpp"
#include "MainConfig.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "SessionStatus.hpp"

#define SOFTWARE_NAME "nginDX"

class Session {
 private:
#ifdef UNIT_TEST
 public:
#endif
  int sock_fd_;
  int file_fd_;
  int retry_count_;
  SessionStatus status_;
  const MainConfig& main_config_;
  const ServerConfig* server_config_;
  const LocationConfig* location_config_;
  Request request_;
  Response response_;
  CgiHandler cgi_handler_;
  pid_t cgi_pid_;
  in_addr_t ip_;
  uint16_t port_;

  Session();
  Session(Session const& other);
  Session& operator=(Session const& other);

  // request
  int receiveRequest();
  bool isMethodAllowed(HTTPMethodFlag method) const;

  // response
  void startCreateResponse();
  void startCreateResponseToGet();
  void createErrorResponse(HTTPStatusCode http_status);
  int sendResponse();

  // load config
  void setupServerAndLocationConfig();
  const ServerConfig* findServer();
  const LocationConfig* findLocation() const;
  bool isLocationMatch(const std::string& loc_route,
                       const std::string& uri_path) const;

  // read from file
  void startReadingFromFile(const std::string& filepath);
  std::string findFileFromDir(const std::string& dirpath) const;
  std::string findFile(const std::string& uri) const;
  std::string findRoot() const;
  bool isIndex(const std::string& filename) const;
  int readFromFile();

  // directrory listing
  void startDirectoryListing(const std::string& filepath);

  // write to file
  void startWritingToFile();
  int writeToFile();

  // cgi process
  void createCgiProcess(const std::string& filepath, const std::string& cgiuri);
  std::string findCgiPathFromUri() const;
  bool isCgiFile(const std::string& filepath) const;
  int writeToCgi();
  int readFromCgi();
  char** storeArgvs();
  const std::vector<std::string> storeMetaVariables(const std::string& cgiuri);
  void storeMetaVariables(const char** meta_variables,
                          const std::vector<std::string>& envp);
  std::string getFromHeaders(const std::map<std::string, std::string>& headers,
                              const std::string key);
  std::string getPathInfo(const std::string& cgiuri);

 public:
  virtual ~Session();
  Session(int sock_fd_, const MainConfig& main_config);

  // getters
  int getSockFd() const;
  int getFileFd() const;
  const SessionStatus& getStatus() const;
  void addResponseHeaderOfFile(const std::string& filepath);

  // functions called from Webserv
  int setFdToSelect(fd_set* rfds, fd_set* wfds);
  int checkSelectedAndExecute(fd_set* rfds, fd_set* wfds);
  int checkReceiveReturn(int ret);
};

#endif /* SESSION_HPP */
