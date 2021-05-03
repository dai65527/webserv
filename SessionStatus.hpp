/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SessionStatus.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/03 10:07:24 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/05/03 08:52:17 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SESSIONSTATUS_HPP
#define SESSIONSTATUS_HPP

enum SessionStatus {
  SESSION_NOT_INIT = 0b00000001,
  SESSION_FOR_CLIENT_RECV = 0b00000010,
  SESSION_FOR_CLIENT_SEND = 0b00000100,
  SESSION_FOR_FILE_READ = 0b00001000,
  SESSION_FOR_FILE_WRITE = 0b00010000,
  SESSION_FOR_CGI_READ = 0b00100000,
  SESSION_FOR_CGI_WRITE = 0b01000000,
  SESSION_FOR_CGI_READ_WRITE = 0b01100000
};

#endif /* SESSIONSTATUS_HPP */
