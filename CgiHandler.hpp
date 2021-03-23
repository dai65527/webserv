/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/03 22:01:23 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/23 15:35:34 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

#include <sys/types.h>

#include <string>

#include "HttpStatusCode.hpp"

#define BUFFER_SIZE 1024

class CgiHandler {
 private:
  pid_t pid_;
  int input_fd_;
  int output_fd_;

  CgiHandler(const CgiHandler& other);
  CgiHandler& operator=(const CgiHandler& other);

 public:
  CgiHandler();
  virtual ~CgiHandler();

  pid_t getPid() const;
  int getInputFd() const;
  int getOutputFd() const;
  // HTTPStatusCode createCgiProcess(const std::string& path);
  HTTPStatusCode createCgiProcess();
  int writeToCgi(const char* buf, size_t size);
  int finishWriting();
  int readFromCgi(char* buf, size_t size);
};

#endif /* CGIHANDLER_HPP */
