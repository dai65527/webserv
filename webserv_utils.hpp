/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv_utils.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 09:06:32 by dnakano           #+#    #+#             */
/*   Updated: 2021/04/12 14:31:17 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_UTILS_HPP
#define WEBSERV_UTILS_HPP

#include <string>

#include "HttpStatusCode.hpp"

HTTPStatusCode isHttpStatusCode(int code);

bool isDirectory(const std::string& path);

size_t getTimeStamp(char* buf, size_t bufsize, const char* fmt);
size_t getTimeStamp(char *buf, size_t bufsize, const char*fmt, time_t unixtime);

std::string basename(const std::string& path);

#endif /* WEBSERV_UTILS_HPP */
