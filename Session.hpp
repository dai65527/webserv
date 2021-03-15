/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/02 01:32:00 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/15 23:10:49 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SESSION_HPP
#define SESSION_HPP

#include <sys/types.h>
#include <unistd.h>

#include "CgiHandler.hpp"
#include "LocationConfig.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "SessionStatus.hpp"

class Session {
 private:
  int sock_fd_;
  int file_fd_;
  int retry_count_;
  const LocationConfig* config_;
  SessionStatus status_;
  Request request_;
  Response response_;
  CgiHandler cgi_handler_;
  pid_t cgi_pid_;

 public:
  Session();
  virtual ~Session();
  Session(int sock_fd_);
  Session(Session const& other);
  Session& operator=(Session const& other);

  int getSockFd() const;
  int getFileFd() const;
  const LocationConfig& getConfig() const;
  const SessionStatus& getStatus() const;
  const Request& getRequest() const;
  const Response& getResponse() const;
  const CgiHandler& getCgiHandler() const;
  void setConfig(const std::list<LocationConfig>& config_list);
  int setFdToSelect(fd_set* rfds, fd_set* wfds);
  int checkSelectedAndExecute(fd_set* rfds, fd_set* wfds);
  void startCreateResponse();

 private:
  int receiveRequest();
  int writeToFile();
  int readFromFile();
  int writeToCgi();
  int readFromCgi();
  int sendResponse();
};

#endif /* SESSION_HPP */
