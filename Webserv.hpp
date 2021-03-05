/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/01 22:44:35 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/05 01:02:19 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <list>
#include <string>

#include "Server.hpp"
#include "Session.hpp"
#include "Socket.hpp"

class Webserv {
 private:
  std::list<Session> sessions_;
  std::list<Socket> sockets_;
  std::list<Server> servers_;

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
