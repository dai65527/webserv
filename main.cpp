/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/10 00:11:38 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/05/06 11:36:48 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <stdexcept>

#include "Webserv.hpp"

bool is_terminating;

void handleSigInt(int sig) {
  (void)sig;
  std::cout << "\b\bWebserv: interrupt: terminating..." << std::endl;
  is_terminating = true;
}

int main(int argc, char** argv) {
  Webserv nginDX;

  // check configfile
  if (argc > 2) {
    std::cout << "webserv: invalid argument number" << std::endl;
    return 1;
  }

  // init signal handlers
  signal(SIGCHLD, SIG_IGN);      // detatch cgi process
  signal(SIGINT, handleSigInt);  // register SIGINT handler

  try {
    nginDX.init(argc == 2 ? argv[1] : "./configfiles/default.conf");
    while (!is_terminating) {
      nginDX.run();
    }
  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
    return 1;
  }
  std::cout << "Bye..." << std::endl;
  return 0;
}
