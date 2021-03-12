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

#endif /* SERVERONLYCONFIGSTORE_HPP */
