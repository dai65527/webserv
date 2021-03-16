/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/15 10:33:40 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/16 11:32:29 by dnakano          ###   ########.fr       */
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
  int fd_;
  size_t char_count_;
  size_t line_count_;

  ConfigParser(const ConfigParser& ref);
  ConfigParser& operator=(const ConfigParser& rhs);
  ConfigParser();

  void throwError(const std::string& err);
  void storeFileContent();
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

 public:
  ConfigParser(const std::string& filename);
  virtual ~ConfigParser();

  // MainConfig parseConfig();
};

#ifdef UNIT_TEST

bool operator==(const ConfigParser::DirectiveNode& lhs,
                const ConfigParser::DirectiveNode& rhs);

#endif /* UNIT_TEST */

#endif /* CONFIGPARSER_HPP */
