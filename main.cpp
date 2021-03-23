/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/10 00:11:38 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/23 14:41:56 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <stdexcept>

#include "Webserv.hpp"

int main(void) {
  Webserv nginDX;

  try {
    nginDX.init();
    while (1) {
      nginDX.run();
    }
  } catch (std::exception e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}