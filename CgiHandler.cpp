/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 23:25:56 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/07 23:29:18 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiHandler.hpp"

CgiHandler(const CgiHandler& other) {}
CgiHandler& operator=(const CgiHandler& other) {}

CgiHandler() {}
virtual ~CgiHandler() {}

pid_t CgiHandler::getPid() const { return pid_; }
int CgiHandler::getInputFd() const { return input_fd_; }
int CgiHandler::getOutputFd() const { return output_fd_; }
int CgiHandler::createCgiProcess(const std::string& path) {}
int CgiHandler::writeToCgi(char* buf, size_t size) {}
int CgiHandler::finishWriting() {}
int CgiHandler::readFromCgi(char* buf, size_t size) {}