/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerOnlyConfigStoreStore.hpp                          :+:      :+:    :+:
 */
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
#ifdef UNIT_TEST
 public:
#endif /* UNIT_TEST */
  std::list<std::pair<in_addr_t, uint16_t> > listen_;  // listen directive
  std::list<std::string> server_name_;                 // server_name directive
  std::map<std::string, in_addr_t> host_ip_map_;       // host ip map

  void addToListen(const in_addr_t addr, const uint16_t port);
  void storeEtcHostsToHostIpMap();
  in_addr_t getIpFromHostIpMap(const char* str);
  in_addr_t parseIp(const char* str);

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

 public:
  // coplien
  ServerOnlyConfigStore();
  ServerOnlyConfigStore(const ServerOnlyConfigStore& ref);
  ServerOnlyConfigStore& operator=(const ServerOnlyConfigStore& ref);
  virtual ~ServerOnlyConfigStore();

  // getters
  const std::list<std::pair<in_addr_t, uint16_t> >& getListen() const;
  const std::list<std::string>& getServerName() const;

  virtual bool parseDirective(const std::string& name,
                              const std::list<std::string>& settings);
};

#endif /* SERVERONLYCONFIGSTORE_HPP */
