/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 10:47:22 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/21 19:50:47 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

ServerConfig::ServerConfig() {}

ServerConfig::ServerConfig(const ServerConfig& ref) { *this = ref; }

ServerConfig& ServerConfig::operator=(const ServerConfig& rhs) {
  locations_ = rhs.locations_;
  CommonConfigStore::operator=(rhs);
  ServerLocationConfigStore::operator=(rhs);
  ServerOnlyConfigStore::operator=(rhs);
  return *this;
}

ServerConfig::~ServerConfig() {}

const std::list<LocationConfig>& ServerConfig::getLocations() const {
  return locations_;
}

void ServerConfig::addLocation(const LocationConfig& location) {
  locations_.push_back(location);
}

bool ServerConfig::parseDirective(const std::string& name,
                                  const std::list<std::string>& settings) {
  if (!ServerOnlyConfigStore::parseDirective(name, settings) &&
      !ServerLocationConfigStore::parseDirective(name, settings) &&
      !CommonConfigStore::parseDirective(name, settings)) {
    throw std::runtime_error(name +
                             ": invalid directive name in server context");
  }
  return true;
}
