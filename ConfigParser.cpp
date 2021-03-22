/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/15 10:34:35 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/21 22:14:07 by dnakano          ###   ########.fr       */
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

ConfigParser::~ConfigParser() {}

MainConfig ConfigParser::parseConfig(const std::string& filename) {
  // open and read config file store it to server
  openAndReadFile(filename);

  // parse config file (may throw exception)
  MainContextNode config_tree = parseMainContext();

  // store parsed data in tree structure (may throw exception)
  return storeMainConfig(config_tree);
}

MainConfig ConfigParser::storeMainConfig(const MainContextNode& main_node) {
  MainConfig main_config;

  // store directives
  for (std::list<DirectiveNode>::const_iterator itr =
           main_node.directives.begin();
       itr != main_node.directives.end(); ++itr) {
    try {
      main_config.parseDirective(itr->name, itr->settings);
    } catch (const std::runtime_error& e) {
      line_count_ = itr->line_no;  // will be in error message
      throwError(e.what());
    }
  }

  // store server contexts
  for (std::list<ServerContextNode>::const_iterator itr =
           main_node.servers.begin();
       itr != main_node.servers.end(); ++itr) {
    main_config.addServer(storeServerCofing(*itr));
  }

  // check config is valid (throw when config is invalid)
  checkMainConfig(main_config);

  return main_config;
}

ServerConfig ConfigParser::storeServerCofing(
    const ServerContextNode& server_node) {
  ServerConfig server_config;

  // store directives
  for (std::list<DirectiveNode>::const_iterator itr =
           server_node.directives.begin();
       itr != server_node.directives.end(); ++itr) {
    try {
      server_config.parseDirective(itr->name, itr->settings);
    } catch (const std::runtime_error& e) {
      line_count_ = itr->line_no;  // will be in error message
      throwError(e.what());
    }
  }

  // store location contexts
  for (std::list<LocationContextNode>::const_iterator itr =
           server_node.locations.begin();
       itr != server_node.locations.end(); ++itr) {
    server_config.addLocation(storeLocationConfig(*itr));
  }

  return server_config;
}

LocationConfig ConfigParser::storeLocationConfig(
    const LocationContextNode& location_node) {
  LocationConfig location_config;

  // store route of location
  location_config.setRoute(location_node.route);

  // store directives
  for (std::list<DirectiveNode>::const_iterator itr =
           location_node.directives.begin();
       itr != location_node.directives.end(); ++itr) {
    try {
      location_config.parseDirective(itr->name, itr->settings);
    } catch (const std::runtime_error& e) {
      line_count_ = itr->line_no;  // will be in error message
      throwError(e.what());
    }
  }

  return location_config;
}

void ConfigParser::checkMainConfig(const MainConfig& main_config) {
  line_count_ = 0;

  // if config has at least one server directive
  if (main_config.getServers().empty()) {
      throwError("missing server directive");
  }

  // check all server has root
  for (std::list<ServerConfig>::const_iterator itr =
           main_config.getServers().begin();
       itr != main_config.getServers().end(); ++itr) {
    if (itr->getListen().empty()) {
      throwError("missing listen directive in server directive");
    } else if (itr->getRoot().empty() && main_config.getRoot().empty()) {
      throwError("missing root directive");
    }
  }
}

ConfigParser::MainContextNode ConfigParser::parseMainContext() {
  MainContextNode res;
  std::string directive_name;

  char_count_ = 0;
  while (1) {
    directive_name = getNextWord();

    if (directive_name.empty()) {
      if (filecontent_[char_count_] == '\0') {
        return res;
      } else {
        throwError("unexpected \'" + std::string(1, filecontent_[char_count_]) +
                   '\'');
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
        char_count_++;
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
    throwError("expected route config of location directive");
  }

  // check '{'
  checkAndSkipOpeningBrace();

  // get directives
  while (1) {
    word = getNextWord();

    if (word.empty()) {
      if (filecontent_[char_count_] == '}') {
        char_count_++;
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
        char_count_++;
        return res;
      } else {
        throwError("expected ';'");
      }
    } else {
      res.settings.push_back(word);
    }
  }
}

void ConfigParser::throwError(const std::string& err) {
  std::string msg;

  msg.reserve(128);
  msg = "webserv: error: ConfigParser: ";
  if (line_count_ != 0) {
    msg += "line " + std::to_string(line_count_) + ": ";
  }
  msg += err;
  throw std::runtime_error(msg);
}

void ConfigParser::openAndReadFile(const std::string& filename) {
  // open file
  int fd = open(filename.c_str(), O_RDONLY);
  if (fd < 0) {
    line_count_ = 0;
    throwError(filename + ": cannot open file");
  }

  // read from file and save to store
  storeFileContent(fd);

  close(fd);
  line_count_ = 1;
}

void ConfigParser::storeFileContent(int fd) {
  char buf[CONF_BUFSIZE];
  ssize_t readlen;

  // read all file content
  while ((readlen = read(fd, buf, CONF_BUFSIZE)) != 0) {
    // check read error
    if (readlen == -1) {
      close(fd);
      throwError(filename_ + ": cannot read file");
    }

    // append buf to filecontent
    try {
      filecontent_.append(buf, readlen);
    } catch (std::exception e) {
      close(fd);
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
  while (filecontent_[char_count_] != '\0') {
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

// for debug
#ifdef UNIT_TEST
#include <ostream>

std::ostream& operator<<(std::ostream& out,
                         const ConfigParser::DirectiveNode& x) {
  out << "line: " << x.line_no << " name: " << x.name << " settings: ";
  std::list<std::string>::const_iterator itr = x.settings.begin();
  while (itr != x.settings.end()) {
    out << *itr;
    ++itr;
    if (itr != x.settings.end()) {
      out << ", ";
    }
  }
  return out;
}

std::ostream& operator<<(std::ostream& out,
                         const ConfigParser::LocationContextNode& x) {
  out << "    line: " << x.line_no << " route: " << x.route << std::endl;
  out << "    directives: " << std::endl;
  for (std::list<ConfigParser::DirectiveNode>::const_iterator itr =
           x.directives.begin();
       itr != x.directives.end(); ++itr) {
    out << "      " << *itr << std::endl;
  }
  return out;
}

std::ostream& operator<<(std::ostream& out,
                         const ConfigParser::ServerContextNode& x) {
  out << "  line: " << x.line_no << std::endl;
  out << "  locations:" << std::endl;
  for (std::list<ConfigParser::LocationContextNode>::const_iterator itr =
           x.locations.begin();
       itr != x.locations.end(); ++itr) {
    out << *itr;
  }
  out << "  directives:" << std::endl;
  for (std::list<ConfigParser::DirectiveNode>::const_iterator itr =
           x.directives.begin();
       itr != x.directives.end(); ++itr) {
    out << "    " << *itr << std::endl;
  }
  return out;
}

std::ostream& operator<<(std::ostream& out,
                         const ConfigParser::MainContextNode x) {
  out << "servers:" << std::endl;
  for (std::list<ConfigParser::ServerContextNode>::const_iterator itr =
           x.servers.begin();
       itr != x.servers.end(); ++itr) {
    out << *itr;
  }
  out << "directives:" << std::endl;
  for (std::list<ConfigParser::DirectiveNode>::const_iterator itr =
           x.directives.begin();
       itr != x.directives.end(); ++itr) {
    out << "  " << *itr << std::endl;
  }
  return out;
}

bool operator==(const ConfigParser::DirectiveNode& lhs,
                const ConfigParser::DirectiveNode& rhs) {
  return (lhs.line_no == rhs.line_no && lhs.name == rhs.name &&
          lhs.settings == rhs.settings);
}

bool operator==(const ConfigParser::LocationContextNode& lhs,
                const ConfigParser::LocationContextNode& rhs) {
  return (lhs.line_no == rhs.line_no && lhs.route == rhs.route &&
          lhs.directives == rhs.directives);
}

bool operator==(const ConfigParser::ServerContextNode& lhs,
                const ConfigParser::ServerContextNode& rhs) {
  return (lhs.line_no == rhs.line_no && lhs.locations == rhs.locations &&
          lhs.directives == rhs.directives);
}

bool operator==(const ConfigParser::MainContextNode& lhs,
                const ConfigParser::MainContextNode& rhs) {
  return (lhs.servers == rhs.servers && lhs.directives == rhs.directives);
}

#endif /* UNIT_TEST */
