/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/05 21:48:48 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/09 20:56:30 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

#include <signal.h>

#include <iostream>
#define DEFALUT_PORT 8080

Webserv::Webserv() {
  int n_fd;     // number of fds ready to read/write (value from select)
  int max_fd;   // maximum nubmer of fds (to pass select())
  fd_set rfds;  // set of read fd
  fd_set wfds;  // set of write fd

  // initialize timeout of select
  struct timeval tv_timeout;  // time to timeout
  tv_timeout.tv_sec = SELECT_TIMEOUT_MS / 1000;
  tv_timeout.tv_usec = (SELECT_TIMEOUT_MS * 1000) % 1000000;

  /* ignore sigchld signal */
  signal(SIGCHLD, SIG_IGN);

  /* prepare sockets according to config */
  // would use while loop to create list and init sockets
  //sockets_(1);//pointerにするか？
	Socket sock;
	sockets_.push_back(sock);
  (sockets_.begin())->init(DEFALUT_PORT, 0);
  std::cout << "socket initialized" << std::endl;

  // main loop
  while (1) {
    // initialize fd sets
    n_fd = 0;
    FD_ZERO(&rfds);
    FD_ZERO(&wfds);

    // set listing socket fd
    FD_SET(sock.getFd(), &rfds);
    max_fd = sock.getFd();  // only one discriptor proveide in this test case

    // set sessions fd
    for (std::list<Session>::iterator itr = sessions_.begin();
         itr != sessions_.end(); ++itr) {
      max_fd = std::max(max_fd, itr->setFdToSelect(&rfds, &wfds));
    }

    // wait for fds getting ready
    std::cout << "selecting..." << std::endl;
    n_fd = select(max_fd + 1, &rfds, &wfds, NULL, &tv_timeout);
    if (n_fd == -1) {
      std::cout << "[error]: select" << std::endl;
    } else if (n_fd == 0) {
      continue;
    }

    // check each session if it is ready to recv/send
    int ret;
    for (std::list<Session>::iterator itr = sessions_.begin();
         itr != sessions_.end() && n_fd > 0;) {
      ret = itr->checkSelectedAndExecute(&rfds, &wfds);
      if (ret == -1)
        itr = sessions_.erase(itr);
      else if (ret == 1)
        n_fd--;
      ++itr;
    }
  }
}

Webserv::~Webserv() {}

Webserv::Webserv(const Webserv& other) {}

Webserv& Webserv::operator=(const Webserv& other) {}

int Webserv::setToSelect() {}

int selectAndExecute() {}