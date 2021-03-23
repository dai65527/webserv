/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/10 00:11:38 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/23 19:23:32 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <stdexcept>

#include "Webserv.hpp"

int main(int argc, char** argv) {
  Webserv nginDX;

  // check configfile
  if (argc != 2) {
    std::cout << "webserv: invalid argument number" << std::endl;
    return 1;
  }

  try {
    nginDX.init(argv[1]);
    while (1) {
      nginDX.run();
    }
  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
  }
  return 1;
}
