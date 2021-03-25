/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/02 01:32:00 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/25 09:22:52 by dnakano          ###   ########.fr       */
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
  int sock_fd_;
  int file_fd_;
  int retry_count_;
  SessionStatus status_;
  const MainConfig& main_config_;
  Request request_;
  Response response_;
  CgiHandler cgi_handler_;
  pid_t cgi_pid_;

  Session();
  Session(Session const& other);
  Session& operator=(Session const& other);

 public:
  virtual ~Session();
  Session(int sock_fd_, const MainConfig& main_config);

  int getSockFd() const;
  int getFileFd() const;
  // いる？
  const LocationConfig& getConfig() const;
  const SessionStatus& getStatus() const;
  const Request& getRequest() const;
  const Response& getResponse() const;
  const CgiHandler& getCgiHandler() const;
  void setConfig(const std::list<LocationConfig>& config_list);
  int setFdToSelect(fd_set* rfds, fd_set* wfds);
  int checkSelectedAndExecute(fd_set* rfds, fd_set* wfds);
  void startCreateResponse();
  int checkResponseType();

 private:
  int receiveRequest();
  void startReadingFromFile();
  void startDirectoryListing();
  void startWritingToFile();
  void startCgiProcess();
  int writeToFile();
  std::string findFile() const;
  std::string findRoot() const;
  std::string findFileFromDir(const std::string& dirpath) const;
  int readFromFile();
  int writeToCgi();
  int readFromCgi();
  int sendResponse();
  void createErrorResponse(HTTPStatusCode http_status);
};

#endif /* SESSION_HPP */
