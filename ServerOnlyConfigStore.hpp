/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerOnlyConfigStoreStore.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 18:59:11 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/12 19:43:23 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERONLYCONFIGSTORE_HPP
#define SERVERONLYCONFIGSTORE_HPP

#include <arpa/inet.h>

#include <list>
#include <map>
#include <string>

/*
** ServerOnlyConfigStore
**
** Base class of ServerConfig
** Includes directives which are only avairable in server context
*/

class ServerOnlyConfigStore {
 protected:
  std::map<in_addr_t, int> listen_;     // listen directive
  std::list<std::string> server_name_;  // server_name directive

 public:
  // coplien
  ServerOnlyConfigStore();
  ServerOnlyConfigStore(const ServerOnlyConfigStore& ref);
  ServerOnlyConfigStore& operator=(const ServerOnlyConfigStore& ref);
  virtual ~ServerOnlyConfigStore();

  // getters
  const std::map<in_addr_t, int>& getListen() const;
  const std::list<std::string>& getServerName() const;

  /*
  ** persers
  **
  ** These function may throw on error.
  **  - syntax error
  **  - wrong value was set
  **  - duplicate value (only for directive cannot be duplicated)
  */

  void parseListen(const std::list<std::string>& settings);
  void parseServerName(const std::list<std::string>& settings);
  
  int checkDirective();
};

#endif /* SERVERONLYCONFIGSTORE_HPP */
