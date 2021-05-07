/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv_utils.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 09:06:32 by dnakano           #+#    #+#             */
/*   Updated: 2021/05/06 10:57:55 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_UTILS_HPP
#define WEBSERV_UTILS_HPP

#include <cstring>
#include <string>

#include "HttpStatusCode.hpp"

extern "C" {
#include "libft.h"
}

HTTPStatusCode isHttpStatusCode(int code);

bool isDirectory(const std::string& path);
size_t getTimeStamp(char* buf, size_t bufsize, const char* fmt);
std::string getIpAddress(uint32_t ip);
int isLitteleEndian();

size_t getTimeStamp(char* buf, size_t bufsize, const char* fmt);
size_t getTimeStamp(char* buf, size_t bufsize, const char* fmt,
                    time_t unixtime);

std::string basename(const std::string& path);
std::string extension(const std::string& filename);

std::string to_string(int n);

#endif /* WEBSERV_UTILS_HPP */
