/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpMethodFlag.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 08:47:06 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/19 09:33:26 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPMETHODFLAG_HPP
#define HTTPMETHODFLAG_HPP

enum HTTPMethodFlag {
  HTTP_GET = 0x001,
  HTTP_HEAD = 0x002,
  HTTP_POST = 0x004,
  HTTP_PUT = 0x008,
  HTTP_DELETE = 0x010,
  HTTP_CONNECT = 0x020,   // CONNECT method is bonus
  HTTP_OPTIONS = 0x040,
  HTTP_TRACE = 0x080
};

#endif /* HTTPMETHODFLAG_HPP */
