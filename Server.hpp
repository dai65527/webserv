/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/01 23:36:35 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/03 10:28:41 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "LocationConfig.hpp"
#include <sys/socket.h>
#include <map>

class Server
{
	private:
		std::map<int, in_addr_t>	port_ip_;
		std::list<std::string>		names_;
		size_t										max_client_body_size_;
		LocationConfig						default_location_config_;

	public:
		Server();
		virtual	~Server();
		Server(Server const &);
		Server&	operator=(Server const &);

		std::map<int, in_addr_t>&	getPortIp() const;
		std::list<std::string>&		getNames() const;
		size_t										getMaxClientBodySize() const;
		LocationConfig&						getDefaultLocationConfig() const;
};

#endif /* SERVER_HPP */
