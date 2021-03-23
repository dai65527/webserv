/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/05 23:34:23 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/23 20:29:56 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <limits>
#include <stdexcept>

/*
** constructor
**
** initialize fd_ and port_ as arguments.
** arguments has to be network order (big endian) and initialize socket
*/

Socket::Socket(in_addr_t ip, uint16_t port) : ip_(ip), port_(port) { init(); }

/*
** destructor
**
** close if socket is opened
*/

Socket::~Socket() {
  if (fd_ > 0) {
    close(fd_);
  }
}

/*
** getters
*/

int Socket::getFd() const { return fd_; }
uint16_t Socket::getPort() const { return port_; }
in_addr_t Socket::getIp() const {return ip_;}
sockaddr_in Socket::getSockAddr() const { return addr_in_; }
socklen_t Socket::getSockLen() const { return addrlen_; }

/*
** function: init
**
** initialize socket
**  - create end point of the socket
**  - create addressing info
**  - bind the address to the socket
**  - make the socket ready to listen
*/

void Socket::init() {
  // create end point of the socket
  //    AF_INET: IPv4 protocol family
  //    SOCK_STREAM: TCP
  //    3rd arg: No need to specify protocols more
  fd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (fd_ < 0) {
    throw std::runtime_error(
        "webserv: error: Socket: init: failed to create socket");
  }

  // change socket to non blocking fd
  if (fcntl(fd_, F_SETFL, O_NONBLOCK) != 0) {
    close(fd_);
    throw std::runtime_error(
        "webserv: error: Socket: init: failed to set to non-blockiing");
  }

  // create addressing info
  addr_in_.sin_family = AF_INET;   // specify address family is IPv4
  addr_in_.sin_addr.s_addr = ip_;  // accept IP address (if all, use INADDR_ANY)
  addr_in_.sin_port = port_;       // convert port number to big endian

  // bind address to the fd of socket
  if (bind(fd_, reinterpret_cast<struct sockaddr *>(&addr_in_),
           sizeof(addr_in_)) == -1) {
    close(fd_);
    throw std::runtime_error(
        "webserv: error: Socket: init: failed to bind socket");
  }

  // make the socket ready to accept connection
  if (listen(fd_, SOCKET_QUE_LEN) == -1) {
    close(fd_);
    throw std::runtime_error(
        "webserv: error: Socket: init: failed to start listiening");
  }

  // initialize address length
  addrlen_ = sizeof(struct sockaddr_in);
}

int Socket::acceptRequest() {
  int accepted_fd;

  // accept connection and store new fd
  accepted_fd =
      accept(fd_, reinterpret_cast<struct sockaddr *>(&addr_in_), &addrlen_);
  if (accepted_fd == -1) {
    std::cout << "[error] failed to accept connection" << std::endl;
    return -1;
  }

  // change fd to non blocking fd
  if (fcntl(fd_, F_SETFL, O_NONBLOCK) != 0) {
    close(accepted_fd);
    throw std::runtime_error("webserv: Socket: cannot initialize socket");
  }

  std::cout << "[webserv] accept connection" << std::endl;
  return accepted_fd;
}
