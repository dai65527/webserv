/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseMainContext.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/14 16:54:52 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/14 17:31:43 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

#include "parseConfig.hpp"

/*
** parseMainContext
*/

MainContextNode parseMainContext(int fd) {
  MainContextNode result;
  std::string stringbuf;
  bool flag_eof;
  size_t line_no;

  // reserve buffersize in advance.
  stringbuf.reserve(1024);

  line_no = 1;
  flag_eof = false;

  // parse in loop
  while (1) {
    // read from file if buffer is empty
    if (stringbuf.empty()) {
      flag_eof = appendNewLine(fd, &stringbuf);
      line_no++;
    }

    // erase heading spaces
    stringbuf.erase(0, countHeadingSpaces(stringbuf));

    switch (checkContext(stringbuf)) {
      case CONF_SERVER:
        result.servers.push_back(parseServerContext(fd, &stringbuf, &line_no));
        break;
      case CONF_LOCATION:
        throw std::runtime_error("webserv: config: syntax error on line " +
                                 std::to_string(line_no) +
                                 ": unexpected \"location\" directive");
        break;
      case CONF_NORMAL_DIRECTIVE:
        result.directives.push_back(
            parseNormalDirective(fd, &stringbuf, &line_no));
        break;
      case CONF_OPENING_BRACE:
        throw std::runtime_error("webserv: config: syntax error on line " +
                                 std::to_string(line_no) +
                                 ": unexpected \"{\"");
        break;
      case CONF_CLOSING_BRACE:
        throw std::runtime_error("webserv: config: syntax error on line " +
                                 std::to_string(line_no) +
                                 ": unexpected \"}\"");
        break;
      case CONF_EMPTY:
        stringbuf.clear();
        break;
      default:
        throw std::runtime_error("webserv: config: syntax error on line " +
                                 std::to_string(line_no) + ": unknown error");
        break;
    }

    if (flag_eof && stringbuf.empty()) {
      return result;
    }
  }
}
