/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/01 22:44:35 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/03 10:32:57 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <list>
#include <string>
#include "Session.hpp"
#include "Socket.hpp"
#include "Server.hpp"

class	Webserv
{
	private:
		std::list<Session>	sessions_;
		std::list<Socket>		sockets_;
		std::list<Server>		servers;
	
		Webserv(Webserv const &);
		Webserv& operator=(Webserv const &);
	public:
		Webserv();
		virtual ~Webserv();

		int		loadConfig(std::string filename) const;
		int		setToSelect();
		int		selectAndExecute();
};

#endif /* WEBSERV_HPP */
