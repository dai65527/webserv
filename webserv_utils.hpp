/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv_utils.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 09:06:32 by dnakano           #+#    #+#             */
/*   Updated: 2021/04/14 00:26:34 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_UTILS_HPP
#define WEBSERV_UTILS_HPP

#include <string>

#include "HttpStatusCode.hpp"

HTTPStatusCode isHttpStatusCode(int code);

bool isDirectory(const std::string& path);
size_t getTimeStamp(char* buf, size_t bufsize, const char* fmt);
std::string getIpAddress(uint32_t ip);
int isLitteleEndian();
#endif /* WEBSERV_UTILS_HPP */
