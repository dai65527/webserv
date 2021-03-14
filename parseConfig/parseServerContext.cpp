/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseServerContext.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/14 17:31:04 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/14 18:29:29 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSESERVERCONTEXT_CPP
#define PARSESERVERCONTEXT_CPP

#include <string>

#include "parseConfig.hpp"

extern "C" {
#include "libft.h"
}

/*
** verifyAndErase
**
** This varifies stringbuf start with "server" followed by "{"
*/

static void verifyAndErase(int fd, std::string* stringbuf, size_t* line_no) {
  bool flag_eof;
  size_t count;

  // check if starts with "server"
  if (stringbuf->compare(0, 6, "server")) {
    throw std::runtime_error("webserv: config: syntax error on line " +
                             std::to_string(*line_no) +
                             ": expected \"server\"");
  }

  // erase "server"
  stringbuf->erase(0, 6);

  // check if followed by "{"
  flag_eof = false;
  while (1) {
    // count charactors to erase
    count = 0;

    // pass spaces
    while (ft_isspace((*stringbuf)[count])) {
      count++;
    }

    // if lines end or comment, read new line
    if ((*stringbuf)[count] == '\0' || (*stringbuf)[count] == '#') {
      // file ended without "}"
      if (flag_eof) {
        throw std::runtime_error("webserv: config: syntax error on line " +
                                 std::to_string(*line_no) + ": expected \"{\"");
      }

      stringbuf->clear();
      flag_eof = appendNewLine(fd, stringbuf);
      (*line_no)++;
      continue;
    }
    break;
  }

  if ((*stringbuf)[count] != '{') {
    throw std::runtime_error("webserv: config: syntax error on line " +
                             std::to_string(*line_no) + ": expected \"{\"");
  }
}

ServerContextNode parseServerContext(int fd, std::string* stringbuf,
                                     size_t* line_no) {
  bool flag_eof;
  char* nextline;
  ServerContextNode result;

  // check and erase "server", following spaces and "{"
  // this may throw exception
  verifyAndErase(fd, stringbuf, line_no);

  // read directives
  while (1) {
    // read from file if buffer is empty
    if (stringbuf->empty()) {
      flag_eof = appendNewLine(fd, stringbuf);
      *line_no++;
    }

    // erase spaces
    stringbuf->erase(0, countHeadingSpaces(*stringbuf));

    // checkContext
    switch (checkContext(*stringbuf)) {
      case CONF_SERVER:
        throw std::runtime_error("webserv: config: syntax error on line " +
                                 std::to_string(*line_no) +
                                 ": unexpected \"server\" directive");
        break;
      case CONF_LOCATION:
        result.locations.push_back(parseLocationContext());
        break;
      case CONF_NORMAL_DIRECTIVE:
        result.directives.push_back(
            parseNormalDirective(fd, &stringbuf, &line_no));
        break;
      case CONF_OPENING_BRACE:
        throw std::runtime_error("webserv: config: syntax error on line " +
                                 std::to_string(*line_no) +
                                 ": unexpected \"{\"");
        break;
      case CONF_CLOSING_BRACE:
        return result;
        break;
      case CONF_EMPTY:
        stringbuf->clear();
        break;
      default:
        throw std::runtime_error("webserv: config: syntax error on line " +
                                 std::to_string(line_no) + ": unknown error");
        break;
    }
  }
}

#endif /* PARSESERVERCONTEXT_CPP */
