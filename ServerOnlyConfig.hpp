#ifndef SERVERONLYCONFIG_HPP
#define SERVERONLYCONFIG_HPP

#include <arpa/inet.h>

#include <list>
#include <map>
#include <string>

/*
** ServerOnlyConfig
**
** Base class of ServerConfig
** Includes directives which are only avairable in server context
*/

class ServerOnlyConfig {
 protected:
  std::map<in_addr_t, int> listen_;     // listen directive
  std::list<std::string> server_name_;  // server_name directive

 public:
  // coplien
  ServerOnlyConfig();
  ServerOnlyConfig(const ServerOnlyConfig& ref);
  ServerOnlyConfig& operator=(const ServerOnlyConfig& ref);
  virtual ~ServerOnlyConfig();

  // getters
  const std::map<in_addr_t, int>& getListen() const;
  const std::list<std::string>& serverName() const;

  /*
  ** persers
  **
  ** Take content settings as string and then parse it and store the results.
  ** Return values:
  **  0: successfully parsed and stored the result
  **  1: syntax error
  **  2: wrong value was set
  **  3: duplicate value (only for directive cannot be duplicated)
  */

  int parseListen(const std::string& settings);
  int parseServerName(const std::string& settings);
};

#endif /* SERVERONLYCONFIG_HPP */
