/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/10 00:11:38 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/12 20:18:28 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <stdexcept>

#include "Webserv.hpp"

int main(void) {
  Webserv ngindx;

  try {
    while (1) {
      ngindx.setToSelect();
      ngindx.selectAndExecute();
    }
  } catch (std::exception e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}