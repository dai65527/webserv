/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/05 21:48:48 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/05/17 09:50:18 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

#include <signal.h>

#include <iostream>

#include "ConfigParser.hpp"
#include "Webserv.hpp"

Webserv::Webserv() : retry_count_(0) {}

Webserv::~Webserv() {
  closeSessions();
  closeSockets();
}

/*
** init and relational functions
*/

void Webserv::init(const std::string& configfilepath) {
  // load configfile
  loadConfig(configfilepath);

  // initialize timeout of select
  tv_timeout_.tv_sec = SELECT_TIMEOUT_MS / 1000;
  tv_timeout_.tv_usec = (SELECT_TIMEOUT_MS * 1000) % 1000000;

  // prepare sockets according to config
  initSockets();
}

void Webserv::loadConfig(const std::string& filename) {
  ConfigParser parser;
  config_ = parser.parseConfig(filename);
}

void Webserv::initSockets() {
  std::list<std::pair<in_addr_t, uint16_t> > listenlist;

  // get unique listen list
  for (std::list<ServerConfig>::const_iterator itr =
           config_.getServers().begin();
       itr != config_.getServers().end(); ++itr) {
    listenlist.insert(listenlist.end(), itr->getListen().begin(),
                      itr->getListen().end());
  }
  listenlist.sort();
  listenlist.unique();

  // add and init sockets
  for (std::list<std::pair<in_addr_t, uint16_t> >::const_iterator itr =
           listenlist.begin();
       itr != listenlist.end();) {
    // add new socket to list
    sockets_.push_back(new Socket(itr->first, itr->second));

    // pass port if INADDR_ANY set
    if (itr->first == INADDR_ANY) {
      while (itr != listenlist.end() &&
             itr->second == sockets_.back()->getPort()) {
        ++itr;
      }
    } else {
      ++itr;
    }
  }
}

void Webserv::run() {
  try {
    if (retry_count_ > 424242) {
      std::cout << "webserv: too many error: restarting webserv..."
                << std::endl;
      resetWebserv();
    } else if (retry_count_ > 42) {
      std::cout << "webserv: too many error: restarting all session..."
                << std::endl;
      resetSessions();
    }
    setToSelect();
    selectAndExecute();
    retry_count_ = 0;
  } catch (const std::exception& e) {
    std::cout << "webserv: run: caught error: " << e.what() << std::endl;
    retry_count_++;
    if (retry_count_ == INT_MAX / 2) {
      throw std::runtime_error("webserv: give up...");
    }
  }
}

int Webserv::setToSelect() {
  // initialize fd sets
  n_fd_ = 0;
  FD_ZERO(&rfds_);
  FD_ZERO(&wfds_);

  // set listing socket fd
  max_fd_ = 0;
  for (std::list<Socket*>::iterator itr = sockets_.begin();
       itr != sockets_.end(); ++itr) {
    FD_SET((*itr)->getFd(), &rfds_);
    max_fd_ = std::max(max_fd_, (*itr)->getFd());
  }

  // set sessions fd
  for (std::list<Session*>::iterator itr = sessions_.begin();
       itr != sessions_.end(); ++itr) {
    max_fd_ = std::max(max_fd_, (*itr)->setFdToSelect(&rfds_, &wfds_));
  }
  return 0;
}

int Webserv::selectAndExecute() {
  // select
  n_fd_ = select(max_fd_ + 1, &rfds_, &wfds_, NULL, &tv_timeout_);
  if (n_fd_ == -1) {
    return 1;
  }

  // check each session if it is ready to recv/send
  int ret;
  for (std::list<Session*>::iterator itr = sessions_.begin();
       itr != sessions_.end();) {
    try {
      ret = (*itr)->checkSelectedAndExecute(&rfds_, &wfds_);
    } catch (const std::exception& e) {
      std::cout << "Webserv: caught exception: " << e.what() << std::endl;
      std::cout << "closing session" << e.what() << std::endl;
      retry_count_++;
    }

    if (ret == -1 || ret == -2) {  // case to close connection
      delete *itr;
      itr = sessions_.erase(itr);
      if (ret == -1) {
        n_fd_--;
      }
    } else {
      ++itr;
      n_fd_ -= ret;
    }
  }

  // accept new connection and add to sessions list
  for (std::list<Socket*>::iterator itr = sockets_.begin();
       itr != sockets_.end() && n_fd_ > 0; ++itr) {
    if (FD_ISSET((*itr)->getFd(), &rfds_)) {
      // accept all incoming connections (rest of n_fd_)
      while (n_fd_-- > 0) {
        int accepted_fd = (*itr)->acceptRequest();
        if (sessions_.size() >= static_cast<size_t>(config_.getMaxSessions())) {
          sessions_.push_back(new Session(accepted_fd, config_, true));
        } else if (accepted_fd >= 0) {
          sessions_.push_back(new Session(accepted_fd, config_, false));
        }
      }
    }
  }
  return 0;
}

void Webserv::closeSessions() {
  for (std::list<Session*>::iterator itr = sessions_.begin();
       itr != sessions_.end(); ++itr) {
    delete *itr;
  }
}

void Webserv::closeSockets() {
  for (std::list<Socket*>::iterator itr = sockets_.begin();
       itr != sockets_.end(); ++itr) {
    delete *itr;
  }
}

void Webserv::resetWebserv() {
  closeSessions();
  closeSockets();
  usleep(100000);  // wait for 0.1 seconds
  initSockets();
}

void Webserv::resetSessions() { closeSessions(); }
