/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/03 22:01:23 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/05 10:03:48 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

#include <sys/types.h>

#include <string>

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
  int getCgiInputFd() const;
  int getCgiOutputFd() const;
  int createCgiProcess(const std::string& path);
  int writeToCgi(char* buf, size_t size);
  int finishWriting();
  int readFromCgi(char* buf, size_t size);
};

#endif /* CGIHANDLER_HPP */
