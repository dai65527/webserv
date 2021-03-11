/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 23:25:56 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/11 11:09:30 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiHandler.hpp"
#include <unistd.h>
#include <iostream>

CgiHandler::CgiHandler() {}
CgiHandler::~CgiHandler() {}

CgiHandler::CgiHandler(const CgiHandler& other) {}
CgiHandler& CgiHandler::operator=(const CgiHandler& other) {}

pid_t CgiHandler::getPid() const { return pid_; }
int CgiHandler::getInputFd() const { return input_fd_; }
int CgiHandler::getOutputFd() const { return output_fd_; }

HTTPStatusCode CgiHandler::createCgiProcess(const std::string& path) {
// create a pipe connect to stdin of cgi process
  int pipe_stdin[2];
  if (pipe(pipe_stdin) == -1) {
    std::cout << "[error] failed to create cgi process" << std::endl;
    return HTTP_500;
  }

  // create a pipe connect to stdout of cgi process
  int pipe_stdout[2];
  if (pipe(pipe_stdout) == -1) {
    std::cout << "[error] failed to create cgi process" << std::endl;
    close(pipe_stdin[0]);
    close(pipe_stdin[1]);
    return HTTP_500;
  }

  // create cgi process
  pid_ = fork();
  if (pid_ == -1) {  // close pipe if failed
    close(pipe_stdin[0]);
    close(pipe_stdin[1]);
    close(pipe_stdout[0]);
    close(pipe_stdout[1]);
    std::cout << "[error] failed to create cgi process" << std::endl;
    return HTTP_500;
  } else if (pid_ == 0) {  // cgi process (child)
    if (dup2(pipe_stdin[0], 0) == -1 || dup2(pipe_stdout[1], 1) == -1) {
      std::cerr << "[error] dup2 failed in cgi process" << std::endl;
      close(0);
      close(pipe_stdin[0]);
      close(pipe_stdin[1]);
      close(pipe_stdout[0]);
      close(pipe_stdout[1]);
      exit(1);
    }

    // close no longer needed pipe fd
    close(pipe_stdin[0]);
    close(pipe_stdin[1]);
    close(pipe_stdout[0]);
    close(pipe_stdout[1]);

    // excecute cgi process (TODO: implement iroiro)
    char* argv[] = {(char*)"/bin/cat", (char*)"-e", NULL};
    execve("/bin/cat", argv, NULL);
    exit(1);
  }

  // save piped fd and set to non blocking
  cgi_input_fd_ = pipe_stdin[1];
  cgi_output_fd_ = pipe_stdout[0];

  // set as non blocking fd
  fcntl(cgi_input_fd_, F_SETFL, O_NONBLOCK);
  fcntl(cgi_output_fd_, F_SETFL, O_NONBLOCK);

  // close fd not to use in parent process
  close(pipe_stdin[0]);
  close(pipe_stdout[1]);

  // change status to cgi write
  status_ = SESSION_FOR_CGI_WRITE;

  // return status 200 on success (but not a final status)
  return HTTP_200;
}

int CgiHandler::writeToCgi(char* buf, size_t size) {}
int CgiHandler::finishWriting() {}
int CgiHandler::readFromCgi(char* buf, size_t size) {}