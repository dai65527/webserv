/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/15 10:34:35 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/15 19:55:21 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <iostream>

extern "C" {
#include "libft.h"
}

ConfigParser::ConfigParser() {}

ConfigParser::ConfigParser(const std::string& filename)
    : filename_(filename), char_count_(0), line_count_(0) {
  // open file
  fd_ = open(filename.c_str(), O_RDONLY);
  if (fd_ < 0) {
    throwError(filename + ": cannot open file");
  }

  // read from file and save to store
  storeFileContent();

  line_count_ = 1;
}

ConfigParser::~ConfigParser() {}

// MainConfig ConfigParser::parseConfig() {
//   // parse config file (may throw exception)
//   MainContextNode ast = parseMainContext();

//   // store parsed data (may throw exception)
//   return storeMainConfig(ast);
// }

// MainContextNode ConfigParser::parseMainContext() {
ConfigParser::MainContextNode ConfigParser::parseMainContext() {
  MainContextNode res;
  std::string directive_name;

  while (1) {
    directive_name = getNextWord();

    if (directive_name.empty()) {
      if (filecontent_[char_count_] == '\0') {
        return res;
      } else {
        throwError("unexpected \"" + std::string(1, filecontent_[char_count_]) +
                   '\"');
        return res;
      }
    } else if (directive_name == "server") {
      res.servers.push_back(parseServerContext());
    } else if (directive_name == "location") {
      throwError("unexpected location directive");
      return res;
    } else {
      res.directives.push_back(parseDirective(directive_name));
    }
  }
}

ConfigParser::ServerContextNode ConfigParser::parseServerContext() {
  std::string word;
  ServerContextNode res;

  // set line_no
  res.line_no = line_count_;

  // check '{'
  checkAndSkipOpeningBrace();

  // get directives
  while (1) {
    word = getNextWord();

    if (word.empty()) {
      if (filecontent_[char_count_] == '}') {
        return res;
      } else {
        throwError("expected '}'");
      }
    } else if (word == "server") {
      throwError("unexpected server directive");
    } else if (word == "location") {
      res.locations.push_back(parseLocationContext());
    } else {
      res.directives.push_back(parseDirective(word));
    }
  }
}

ConfigParser::LocationContextNode ConfigParser::parseLocationContext() {
  std::string word;
  LocationContextNode res;

  // set line_no
  res.line_no = line_count_;

  // get rocation
  res.route = getNextWord();
  if (res.route.empty()) {
    throwError("route expected");
  }

  // check '{'
  checkAndSkipOpeningBrace();

  // get directives
  while (1) {
    word = getNextWord();

    if (word.empty()) {
      if (filecontent_[char_count_] == '}') {
        return res;
      } else {
        throwError("expected '}'");
      }
    } else if (word == "server") {
      throwError("unexpected server directive");
    } else if (word == "location") {
      throwError("unexpected location directive");
    } else {
      res.directives.push_back(parseDirective(word));
    }
  }
}

ConfigParser::DirectiveNode ConfigParser::parseDirective(
    const std::string& directive_name) {
  std::string word;
  DirectiveNode res;

  res.name = directive_name;
  res.line_no = line_count_;

  while (1) {
    word = getNextWord();

    if (word.empty()) {
      if (filecontent_[char_count_] == ';') {
        return res;
      } else {
        throwError("expected ';'");
      }
    } else {
      res.settings.push_back(word);
    }
  }
}

// MainConfig ConfigParser::storeMainConfig(
//     const ConfigParser::MainContextNode& main_node) {
//   (void)main_node;
//   return MainConfig();
// }

void ConfigParser::throwError(const std::string& err) {
  std::string msg;

  msg.reserve(128);
  msg = "webserv: config: ";
  if (line_count_ != 0) {
    msg += "line: " + std::to_string(line_count_) + ": ";
  }
  msg += err;
  throw std::runtime_error(msg);
}

void ConfigParser::storeFileContent() {
  char buf[CONF_BUFSIZE];
  ssize_t readlen;

  // read all file content
  while ((readlen = read(fd_, buf, CONF_BUFSIZE)) != 0) {
    // check read error
    if (readlen == -1) {
      close(fd_);
      throwError(filename_ + ": cannot read file");
    }

    // append buf to filecontent
    try {
      filecontent_.append(buf, readlen);
    } catch (std::exception e) {
      close(fd_);
      throwError(filename_ + ": file too large");
    }
  }
}

bool ConfigParser::isSpecialChar(char c) const {
  return (c == '\0' || c == '{' || c == '}' || c == ';' || c == '#' ||
          ft_isspace(c));
}

void ConfigParser::skipSpace() {
  while (ft_isspace(filecontent_[char_count_])) {
    if (filecontent_[char_count_] == '\n') {
      line_count_++;
    }
    char_count_++;
  }
}

void ConfigParser::skipComment() {
  if (filecontent_[char_count_] != '#') {
    return;
  }

  char_count_++;
  while (filecontent_[char_count_] != '\0' &&
         filecontent_[char_count_] != '\n') {
    char_count_++;
  }
}

void ConfigParser::skipSpaceAndComment() {
  while (1) {
    skipSpace();
    if (filecontent_[char_count_] != '#') {
      break;
    }
    skipComment();
  }
}

void ConfigParser::checkAndSkipOpeningBrace() {
  skipSpaceAndComment();
  if (filecontent_[char_count_] != '{') {
    throwError("expected '{'");
  }
  char_count_++;
}

std::string ConfigParser::getNextWord() {
  size_t char_count_origin;

  skipSpaceAndComment();
  char_count_origin = char_count_;
  while (!isSpecialChar(filecontent_[char_count_])) {
    char_count_++;
  }
  return filecontent_.substr(char_count_origin,
                             char_count_ - char_count_origin);
}
