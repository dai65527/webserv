/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/05 23:34:23 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/19 02:14:57 by dhasegaw         ###   ########.fr       */
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
** default constructor
**
** initialize fd_ and port_ by 0
** will check value in each member functions
*/

Socket::Socket() : fd_(0), port_(0) {}

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
int Socket::getPort() const { return port_; }
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

void Socket::init(int port, int ip) {
  // check the range of port
  if (port < 0 || std::numeric_limits<unsigned short>::max() < port) {
    throw std::runtime_error("webserv: Socket: port nubmer is out of range");
  }

  // create end point of the socket
  //    AF_INET: IPv4 protocol family
  //    SOCK_STREAM: TCP
  //    3rd arg: No need to specify protocols more
  fd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (fd_ < 0) {
    throw std::runtime_error("webserv: Socket: cannot initialize socket");
  }

  // change socket to non blocking fd
  if (fcntl(fd_, F_SETFL, O_NONBLOCK) != 0) {
    close(fd_);
    throw std::runtime_error("webserv: Socket: cannot initialize socket");
  }

  // create addressing info
  addr_in_.sin_family = AF_INET;  // specify address family is IPv4
  addr_in_.sin_addr.s_addr = INADDR_ANY;  // accept IP address (if all, use INADDR_ANY)
  addr_in_.sin_port = htons(port);  // convert port number to big endian
                                    // TODO: need to create "ft_htons"
  // bind address to the fd of socket
  if (bind(fd_, reinterpret_cast<struct sockaddr *>(&addr_in_),
           sizeof(addr_in_)) == -1) {
    close(fd_);
    throw std::runtime_error("webserv: Socket: cannot initialize socket");
  }
  
  // make the socket ready to accept connection
  if (listen(fd_, SOCKET_QUE_LEN) == -1) {
    close(fd_);
    throw std::runtime_error("webserv: Socket: cannot initialize socket");
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
