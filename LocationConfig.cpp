/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 18:56:57 by dnakano           #+#    #+#             */
/*   Updated: 2021/04/06 14:49:36 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationConfig.hpp"

LocationConfig::LocationConfig() {}
LocationConfig::LocationConfig(const LocationConfig& ref) { *this = ref; }
LocationConfig::~LocationConfig() {}

LocationConfig& LocationConfig::operator=(const LocationConfig& rhs) {
  if (this != &rhs) {
    CommonConfigStore::operator=(rhs);
    ServerLocationConfigStore::operator=(rhs);
    route_ = rhs.route_;
  }
  return *this;
}

/*** getter ***/
const std::string& LocationConfig::getRoute() const { return route_; }

/*** setter ***/
void LocationConfig::setRoute(const std::string& route) { route_ = route; }

bool LocationConfig::parseDirective(const std::string& name,
                                    const std::list<std::string>& settings) {
  if (!CommonConfigStore::parseDirective(name, settings) &&
      !ServerLocationConfigStore::parseDirective(name, settings)) {
    throw std::runtime_error(name +
                             ": invalid directive name in location context");
  }
  return true;
}
