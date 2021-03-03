/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/02 01:32:00 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/03 10:51:07 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SESSION_HPP
#define SESSION_HPP

#include "LocationConfig.hpp"
#include "SessionStatus.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "CgiHandler.hpp"
#include <sys/types.h> 
#include <unistd.h>

class	Session
{
	private:
		int		sock_fd_;
		const LocationConfig&	config_;
		SessionStatus	status_;
		Request	request_;
		Response response_;
		CgiHandler cgi_handler_;
		pid_t cgi_pid;
	
	public:
		Session();
		virtual ~Session();
		Session(int sock_fd_);
		Session(Session const & other);
		Session& operator=(Session const & other);

		int	getSockFd() const;
		LocationConfig&	getConfig() const;
		SessionStatus	getStatus() const;
		Request	getRequest() const;
		Response getResponse() const;
		CgiHandler getCgiHandler() const;
		pid_t getCgiPid() const;
		void	setConfig();
		int	setFdToSelect(int rfds, int wfds);
		int	checkSelectedAndExecute()
		void startCreateResponse();
	private:
		int	writeToFile();
		int readToFromFile() const;
		int sendResponse() const;
}


#endif /* SESSION_HPP */
