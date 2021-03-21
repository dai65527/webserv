/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 18:58:48 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/21 18:06:19 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <list>
#include <string>

#include "CommonConfigStore.hpp"
#include "LocationConfig.hpp"
#include "ServerLocationConfigStore.hpp"
#include "ServerOnlyConfigStore.hpp"

class ServerConfig : public CommonConfigStore,
                     public ServerLocationConfigStore,
                     public ServerOnlyConfigStore {
 private:
  std::list<LocationConfig> locations_;

 public:
  ServerConfig();
  ServerConfig(const ServerConfig& ref);
  ServerConfig& operator=(const ServerConfig& rhs);
  virtual ~ServerConfig();

  const std::list<LocationConfig>& getLocations() const;

  void addLocation(const LocationConfig& location);
  bool parseDirective(const std::string& name,
                      const std::list<std::string>& settings);
};

#endif /* SERVERCONFIG_HPP */
