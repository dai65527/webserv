/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/01 23:24:47 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/19 02:13:24 by dhasegaw         ###   ########.fr       */
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
  int port_;
  sockaddr_in addr_in_;
  socklen_t addrlen_;

 public:
  Socket(Socket const&);
  Socket& operator=(Socket const&);

  //  public:
  Socket();
  virtual ~Socket();

  int getFd() const;
  int getPort() const;
  sockaddr_in getSockAddr() const;
  socklen_t getSockLen() const;
  void init(int port, int ip);
  int acceptRequest();
};

#endif /* SOCKET_HPP */
