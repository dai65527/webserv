/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/01 22:44:35 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/24 08:12:58 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <list>
#include <string>

#include "Session.hpp"
#include "Socket.hpp"
#include "MainConfig.hpp"

#define SELECT_TIMEOUT_MS 2500

class Webserv {
 private:
  std::list<Session*> sessions_;
  std::list<Socket*> sockets_;
  MainConfig config_;
  int n_fd_;
  int max_fd_;
  fd_set rfds_;
  fd_set wfds_;
  struct timeval tv_timeout_;

  Webserv(const Webserv& other);
  Webserv& operator=(const Webserv& other);

  /*** functions used in init function ***/
  void loadConfig(const std::string& filename);
  void initSockets();

  int setToSelect();
  int selectAndExecute();

 public:
  Webserv();
  virtual ~Webserv();

  void init(const std::string& configfilepath);
  void run();
};

#endif /* WEBSERV_HPP */
