/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/05 21:48:48 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/16 00:04:36 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

#include <signal.h>

#include <iostream>
#define DEFALUT_PORT 8080

Webserv::Webserv() {
  // initialize timeout of select
  tv_timeout_.tv_sec = SELECT_TIMEOUT_MS / 1000;
  tv_timeout_.tv_usec = (SELECT_TIMEOUT_MS * 1000) % 1000000;

  /* ignore sigchld signal */
  signal(SIGCHLD, SIG_IGN);

  /* prepare sockets according to config */
  // would use while loop to create list and init sockets
  // sockets_(1);//pointerにするか？
  Socket* sock = new Socket;
  sockets_.push_back(sock);
  (*sockets_.begin())->init(DEFALUT_PORT, 0);
  std::cout << "socket initialized" << std::endl;
}

Webserv::~Webserv() {
  for (std::list<Session*>::iterator itr = sessions_.begin();
       itr != sessions_.end(); ++itr) {
    delete *itr;
  }

  for (std::list<Socket*>::iterator itr = sockets_.begin();
       itr != sockets_.end(); ++itr) {
    delete *itr;
  }

  for (std::list<Server*>::iterator itr = servers_.begin();
       itr != servers_.end(); ++itr) {
    delete *itr;
  }
}

Webserv::Webserv(const Webserv& other) {}

Webserv& Webserv::operator=(const Webserv& other) {}

int Webserv::setToSelect() {
  // initialize fd sets
  n_fd_ = 0;
  FD_ZERO(&rfds_);
  FD_ZERO(&wfds_);

  // set listing socket fd
  for (std::list<Socket*>::iterator itr = sockets_.begin();
       itr != sockets_.end(); ++itr) {
    FD_SET((*itr)->getFd(), &rfds_);
    max_fd_ = (*itr)->getFd();
  }

  // set sessions fd
  for (std::list<Session*>::iterator itr = sessions_.begin();
       itr != sessions_.end(); ++itr) {
    max_fd_ = std::max(max_fd_, (*itr)->setFdToSelect(&rfds_, &wfds_));
  }
}

int Webserv::selectAndExecute() {
  std::cout << "selecting..." << std::endl;
  n_fd_ = select(max_fd_ + 1, &rfds_, &wfds_, NULL, &tv_timeout_);
  if (n_fd_ == -1) {
    std::cout << "[error]: select" << std::endl;
  } else if (n_fd_ == 0) {
    return (1);
  }

  // check each session if it is ready to recv/send
  int ret;
  for (std::list<Session*>::iterator itr = sessions_.begin();
       itr != sessions_.end() && n_fd_ > 0;) {
    ret = (*itr)->checkSelectedAndExecute(&rfds_, &wfds_);
    if (ret == -1)
      itr = sessions_.erase(itr);
    else if (ret == 1)
      n_fd_--;
    ++itr;
  }

  // accept new connection and add to sessions list
  for (std::list<Socket*>::iterator itr = sockets_.begin();
       itr != sockets_.end() && n_fd_ > 0; ++itr) {
    if (FD_ISSET((*itr)->getFd(), &rfds_)) {
      int accepted_fd;
      // accept all incoming connections (rest of n_fd_)
      while (n_fd_-- > 0) {
        accepted_fd = (*itr)->acceptRequest();
        if (accepted_fd >= 0) {
          Session* sess = new Session(accepted_fd);
          sessions_.push_back(sess);
        }
      }
    }
  }
	return 0;
}
