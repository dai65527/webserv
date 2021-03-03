/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/01 23:24:47 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/03 10:31:18 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <sys/socket.h>
#include <netinet/in.h> 
#include "Session.hpp"

class	Socket
{
	private:
		int					fd_;
		int					port_;
		sockaddr_in	addr_in_;
		socklen_t		addrlen_;

		Socket(Socket const &);
		Socket&	operator=(Socket const &);
	public:
		Socket();
		virtual	~Socket();

		int					getFd() const;
		int					getPort() const;
		sockaddr_in	getSockAddr() const;
		socklen_t		getSockLen() const;
		void				init(int	port) const;
		Session& 		acceptRequest() const;
};

#endif /* SOCKET_HPP */
