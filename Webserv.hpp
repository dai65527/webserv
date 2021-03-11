/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/01 22:44:35 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/10 01:13:37 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <list>
#include <string>

#include "Server.hpp"
#include "Session.hpp"
#include "Socket.hpp"

#define SELECT_TIMEOUT_MS 2500

class Webserv {
 private:
  std::list<Session*> sessions_;
  std::list<Socket*> sockets_;
  std::list<Server*> servers_;
  int n_fd_;
  int max_fd_;
  fd_set rfds_;
  fd_set wfds_;
  struct timeval tv_timeout_;

  Webserv(const Webserv& other);
  Webserv& operator=(const Webserv& other);

 public:
  Webserv();
  virtual ~Webserv();

  int loadConfig(const std::string& filename) const;
  int setToSelect();
  int selectAndExecute();
};

#endif /* WEBSERV_HPP */
