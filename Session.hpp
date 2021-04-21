/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/02 01:32:00 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/04/21 09:09:04 by dnakano          ###   ########.fr       */
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

class Session {
 private:
#ifdef UNIT_TEST
 public:
#endif
  static std::map<std::string, std::string> map_mime_ext_;
  static std::map<std::string, std::string> map_ext_mime_;
  static void initMapMineExt();

  int sock_fd_;
  int file_fd_;
  long time_last_connect_;
  int retry_count_;
  bool flg_exceed_max_session_;
  SessionStatus status_;
  const MainConfig& main_config_;
  const ServerConfig* server_config_;
  const LocationConfig* location_config_;
  Request request_;
  Response response_;
  CgiHandler cgi_handler_;
  pid_t cgi_pid_;  // is this needed???

  Session();
  Session(Session const& other);
  Session& operator=(Session const& other);

  // connection
  void resetAll();
  bool checkConnectionTimeOut() const;
  void updateConnectionTime();

  // request
  int receiveRequest();
  bool isMethodAllowed(HTTPMethodFlag method) const;

  // response
  void startCreateResponse();
  void startCreateResponseToGet();
  void startCreateResponseToPost();
  void createErrorResponse(HTTPStatusCode http_status);
  int sendResponse();

  // load config
  void setupServerAndLocationConfig();
  const ServerConfig* findServer() const;
  const LocationConfig* findLocation() const;
  bool isLocationMatch(const std::string& loc_route,
                       const std::string& uri_path) const;

  // read from file
  void startReadingFromFile(const std::string& filepath);
  std::string findFileFromDir(const std::string& dirpath) const;
  std::string findFile(const std::string& uri) const;
  const std::string& findRoot() const;
  bool isIndex(const std::string& filename) const;
  int readFromFile();
  int addResponseHeaderOfFile(const std::string& filepath);

  // directrory listing
  void startDirectoryListing(const std::string& filepath);
  bool isAutoIndexOn() const;
  int createFileList(const std::string& dirpath,
                     std::list<struct FileInfo>* files) const;
  void createDirectoryListingHeader();
  void createDirectoryListingBody(const std::list<struct FileInfo>& files);
  void createDirectoryList(const std::list<struct FileInfo>& files);

  // cgi process
  void createCgiProcess(const std::string& filepath, const std::string& cgiuri);
  std::string findCgiPathFromUri() const;
  bool isCgiFile(const std::string& filepath) const;
  int writeToCgi();
  int readFromCgi();

  // write to file
  void startWritingToFile();
  std::string findUploadStore(const std::string& uri) const;
  std::string createFilename() const;
  std::string getFileExtension() const;
  int writeToFile();

  // respond to options header
  void startCreateResponseToOptions();
  void createAllowHeader();

 public:
  virtual ~Session();
  Session(int sock_fd_, const MainConfig& main_config,
          bool flg_exceed_max_session);

  // getters
  int getSockFd() const;
  int getFileFd() const;
  const SessionStatus& getStatus() const;

  // functions called from Webserv
  int setFdToSelect(fd_set* rfds, fd_set* wfds);
  int checkSelectedAndExecute(fd_set* rfds, fd_set* wfds);
  int checkReceiveReturn(int ret);
};

#endif /* SESSION_HPP */
