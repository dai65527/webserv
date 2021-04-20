/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/03 22:01:23 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/04/21 01:33:19 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

#include <sys/types.h>

#include <string>
#include <vector>

#include "HttpStatusCode.hpp"

#define BUFFER_SIZE 1024

class CgiHandler {
 private:
  pid_t pid_;
  int input_fd_;
  int output_fd_;
  std::vector<char> buf_;

  CgiHandler(const CgiHandler& other);
  CgiHandler& operator=(const CgiHandler& other);

 public:
  CgiHandler();
  virtual ~CgiHandler();

  pid_t getPid() const;
  int getInputFd() const;
  int getOutputFd() const;
  const std::vector<char>& getBuf() const;
  HTTPStatusCode createCgiProcess(const std::string& filepath, char** argv,
                                  char** meta_variables);
  void storeMetaVariables(const char* meta_variables[],
                          const std::vector<std::string>& meta_variables_str);
  int writeToCgi(const char* buf, size_t size);
  int finishWriting();
  int readFromCgi();
};

#endif /* CGIHANDLER_HPP */
