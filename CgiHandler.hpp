/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/03 22:01:23 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/03 22:12:30 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

#include <sys/types.h> 

class CgiHandler
{
	private:
		pid_t	pid_;
		int	cgi_input_fd_;
		int	cgi_output_fd_;

		CgiHandler(CgiHandler const & other);
		CgiHandler& operator=(CgiHandler const & other);
	public:
		CgiHandler();
		virtual ~CgiHandler();

		pid_t	getPid() const;
		int	getCgiInputFd() const;
		int	getCgiOutputFd() const;
};

#endif /* CGIHANDLER_HPP */


#endif /* CGIHANDLER_HPP */
