/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv_utils.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 09:06:32 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/19 08:39:34 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_UTILS_HPP
#define WEBSERV_UTILS_HPP

#include "HttpStatusCode.hpp"

HTTPStatusCode isHttpStatusCode(int code);

#endif /* WEBSERV_UTILS_HPP */
