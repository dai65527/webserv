/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/02 01:32:00 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/04/20 11:33:12 by dhasegaw         ###   ########.fr       */
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
#define SESS_BEFORE_PARSE_CGI_HEADER 0
#define SESS_FIN_PARSE_CGI_HEADER 1

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
  int cgiResponseParseProgress_;

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
  const ServerConfig* findServer();
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
  char** storeArgv(const std::string& filepath, const std::string& cgiuri);
  char** storeMetaVariables(const std::string& cgiuri);
  std::string getFromHeaders(const std::map<std::string, std::string>& headers,
                             const std::string key) const;
  std::string getPathInfo(const std::string& cgiuri) const;
  char** vecToChar(std::vector<std::string>& meta_variables);
  ssize_t parseReadBuf(char* read_buf, ssize_t n);

  // write to file
  void startWritingToFile();
  std::string findUploadStore(const std::string& uri) const;
  std::string createFilename() const;
  std::string getFileExtension() const;
  int writeToFile();

 public:
  virtual ~Session();
  Session(int sock_fd_, const MainConfig& main_config);

  // getters
  int getSockFd() const;
  int getFileFd() const;
  in_addr_t getIp() const;
  uint16_t getPort() const;
  
  const SessionStatus& getStatus() const;
  void addResponseHeaderOfFile(const std::string& filepath);

  // functions called from Webserv
  int setFdToSelect(fd_set* rfds, fd_set* wfds);
  int checkSelectedAndExecute(fd_set* rfds, fd_set* wfds);
  int checkReceiveReturn(int ret);

  class CgiParams {
   private:
    const Session& session_;
    char** argv_;
    char** envp_;

    CgiParams();
    CgiParams(CgiParams const& other);
    CgiParams& operator=(CgiParams const& other);

   public:
    CgiParams(Session const& session);
    ~CgiParams();

    char** storeArgv(const std::string& filepath, const std::string& cgiuri,
                     const Request& request);
    char** storeMetaVariables(const std::string& cgiuri,
                              const Request& request);

   private:
    char** vecToChar(std::vector<std::string>& meta_variables);
  };
};

#endif /* SESSION_HPP */
