/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/01 23:24:47 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/23 19:38:19 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <netinet/in.h>
#include <sys/socket.h>
#define SOCKET_QUE_LEN 128

class Socket {
 private:
  int fd_;
  uint16_t port_;
  in_addr_t ip_;
  sockaddr_in addr_in_;
  socklen_t addrlen_;

  Socket();
  Socket(Socket const&);
  Socket& operator=(Socket const&);

  void init();

 public:
  Socket(in_addr_t ip, uint16_t port);
  virtual ~Socket();

  int getFd() const;
  uint16_t getPort() const;
  in_addr_t getIp() const;
  sockaddr_in getSockAddr() const;
  socklen_t getSockLen() const;
  int acceptRequest();
};

#endif /* SOCKET_HPP */
