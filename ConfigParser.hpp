/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/15 10:33:40 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/21 20:40:59 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <list>
#include <string>

#include "MainConfig.hpp"

#define CONF_BUFSIZE 1024

class ConfigParser {
 private:
#ifdef UNIT_TEST
 public:
#endif /* UNIT_TEST */

  enum ConfigDirectiveType {
    CONF_SERVER,
    CONF_LOCATION,
    CONF_NORMAL_DIRECTIVE,
    CONF_OPENING_BRACE,
    CONF_CLOSING_BRACE,
    CONF_EOF
  };

  /*
  ** AST nodes
  */

  struct DirectiveNode {
    int line_no;
    std::string name;
    std::list<std::string> settings;
  };

  struct LocationContextNode {
    int line_no;
    std::string route;
    std::list<DirectiveNode> directives;
  };

  struct ServerContextNode {
    int line_no;
    std::list<DirectiveNode> directives;
    std::list<LocationContextNode> locations;
  };

  struct MainContextNode {
    std::list<DirectiveNode> directives;
    std::list<ServerContextNode> servers;
  };

  std::string filename_;
  std::string filecontent_;
  size_t char_count_;
  size_t line_count_;

  ConfigParser(const ConfigParser& ref);
  ConfigParser& operator=(const ConfigParser& rhs);

  void throwError(const std::string& err);
  void storeFileContent(int fd);
  void openAndReadFile(const std::string& filename);
  bool isSpecialChar(char c) const;
  void skipSpace();
  void skipComment();
  void skipSpaceAndComment();
  void checkAndSkipOpeningBrace();
  std::string getNextWord();

  MainContextNode parseMainContext();
  ServerContextNode parseServerContext();
  LocationContextNode parseLocationContext();
  DirectiveNode parseDirective(const std::string& directive_name);

  MainConfig storeMainConfig(const MainContextNode& main_node);
  ServerConfig storeServerCofing(const ServerContextNode& server_node);
  LocationConfig storeLocationConfig(const LocationContextNode& location_node);

  void checkMainConfig(const MainConfig& main_config);

 public:
  ConfigParser();
  virtual ~ConfigParser();

  MainConfig parseConfig(const std::string& filename);
};

// for test and debug
#ifdef UNIT_TEST

bool operator==(const ConfigParser::DirectiveNode& lhs,
                const ConfigParser::DirectiveNode& rhs);

bool operator==(const ConfigParser::LocationContextNode& lhs,
                const ConfigParser::LocationContextNode& rhs);

bool operator==(const ConfigParser::ServerContextNode& lhs,
                const ConfigParser::ServerContextNode& rhs);

bool operator==(const ConfigParser::MainContextNode& lhs,
                const ConfigParser::MainContextNode& rhs);

#include <ostream>

std::ostream& operator<<(std::ostream& out,
                         const ConfigParser::DirectiveNode& x);

std::ostream& operator<<(std::ostream& out,
                         const ConfigParser::LocationContextNode& x);

std::ostream& operator<<(std::ostream& out,
                         const ConfigParser::ServerContextNode& x);

std::ostream& operator<<(std::ostream& out,
                         const ConfigParser::LocationContextNode& x);

std::ostream& operator<<(std::ostream& out,
                         const ConfigParser::MainContextNode x);

#endif /* UNIT_TEST */

#endif /* CONFIGPARSER_HPP */
