/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SessionStatus.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/03 10:07:24 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/05 20:29:03 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SESSIONSTATUS_HPP
#define SESSIONSTATUS_HPP

enum SessionStatus {
  SESSION_NOT_INIT,
  SESSION_FOR_CLIENT_RECV,
  SESSION_FOR_CLIENT_SEND,
  SESSION_FOR_FILE_READ,
  SESSION_FOR_FILE_WRITE,
  SESSION_FOR_CGI_READ,
  SESSION_FOR_CGI_WRITE
};

#endif /* SESSIONSTATUS_HPP */
