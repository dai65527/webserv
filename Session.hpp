/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/02 01:32:00 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/04/30 23:04:13 by dhasegaw         ###   ########.fr       */
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
#include "webserv_utils.hpp"

#define SOFTWARE_NAME "nginDX"

class Request;

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
  in_addr_t ip_;
  uint16_t port_;

  // this is set in createErrorResponse function
  // to save
  HTTPStatusCode original_error_response_;

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
  void startCreateResponseToPut();
  void startCreateResponseToTrace();
  void createErrorResponse(HTTPStatusCode http_status);
  int createErrorResponseFromFile(HTTPStatusCode http_status);
  std::string findErrorPage(HTTPStatusCode http_status) const;
  int sendResponse();

  // load config
  const ServerConfig* findServer();
  const LocationConfig* findLocation() const;
  bool isLocationMatch(const std::string& loc_route,
                       const std::string& uri_path) const;

  // read from file
  void startReadingFromFile(const std::string& filepath);
  std::string findFileFromDir(const std::string& dirpath) const;
  std::string findFile(const std::string& uri) const;
  std::string mimeType(const std::string& filepath) const;
  bool isCharsetAccepted(const std::string& mime_type) const;
  bool isLanguageAccepted() const;
  int addResponseHeaderOfFile(const std::string& filepath,
                              const std::string& mime_type);
  void addContentTypeHeader(const std::string& filepath,
                            const std::string& mime_type);
  void addContentLanguageHeader();
  std::string findCharset() const;
  const std::list<std::string>& findLanguage() const;
  bool isIndex(const std::string& filename) const;
  int readFromFile();

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
  ssize_t parseReadBuf(const char* read_buf, ssize_t n);

  // write to file
  void startWritingToFile(const std::string& filepath);
  std::string getUploadFilePath();
  std::string findUploadStore(const std::string& uri) const;
  std::string findUploadPass(const std::string& uri) const;
  std::string createFilename() const;
  std::string findFileNameFromUri() const;
  std::string getFileExtension() const;
  int writeToFile();

  // respond to options header
  void startCreateResponseToOptions();
  void createAllowHeader();

  std::string getUriFromLocation(std::string uri = "") const;

 public:
  virtual ~Session();
  Session(int sock_fd_, const MainConfig& main_config,
          bool flg_exceed_max_session = false);

  // getters
  int getSockFd() const;
  int getFileFd() const;
  in_addr_t getIp() const;
  uint16_t getPort() const;
  const SessionStatus& getStatus() const;
  std::string getFromHeaders(const std::map<std::string, std::string>& headers,
                             const std::string key) const;
  std::string getPathInfo(const std::string& cgiuri) const;

  // functions called from Webserv
  int setFdToSelect(fd_set* rfds, fd_set* wfds);
  int checkSelectedAndExecute(fd_set* rfds, fd_set* wfds);
  int checkReceiveReturn(int ret);

  // load config
  const std::string& findRoot() const;
  void setupServerAndLocationConfig();
  unsigned long getClientMaxBodySize() const;
};

#endif /* SESSION_HPP */
