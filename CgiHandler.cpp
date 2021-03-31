/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 23:25:56 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/30 19:09:35 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiHandler.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <iostream>

#include "HttpStatusCode.hpp"

CgiHandler::CgiHandler() {}
CgiHandler::~CgiHandler() {}

pid_t CgiHandler::getPid() const { return pid_; }
int CgiHandler::getInputFd() const { return input_fd_; }
int CgiHandler::getOutputFd() const { return output_fd_; }

// HTTPStatusCode CgiHandler::createCgiProcess(const std::string& path) {
HTTPStatusCode CgiHandler::createCgiProcess() {
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
  input_fd_ = pipe_stdin[1];
  output_fd_ = pipe_stdout[0];

  // set as non blocking fd
  fcntl(input_fd_, F_SETFL, O_NONBLOCK);
  fcntl(output_fd_, F_SETFL, O_NONBLOCK);

  // close fd not to use in parent process
  close(pipe_stdin[0]);
  close(pipe_stdout[1]);

  // change status to cgi write
  // status_ = SESSION_FOR_CGI_WRITE;

  // return status 200 on success (but not a final status)
  return HTTP_200;
}

int CgiHandler::writeToCgi(const char* buf, size_t size) {
  // write to cgi process
  return (write(input_fd_, buf, size));
}

// int CgiHandler::finishWriting() {}

int CgiHandler::readFromCgi(char* buf, size_t size) {
  ssize_t ret;

  // read from cgi process
  ret = read(output_fd_, buf, size);

  // retry seveal times even if read failed
  if (ret == -1) {
    return -1;
  }
  // check if pipe closed
  if (ret == 0) {
    close(output_fd_);  // close pipefd
    return 0;
  }
  return ret;
}
