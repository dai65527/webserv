/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 18:56:57 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/19 11:17:45 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationConfig.hpp"

LocationConfig::LocationConfig() {}
LocationConfig::LocationConfig(const LocationConfig& ref) { *this = ref; }
LocationConfig::~LocationConfig() {}

LocationConfig& LocationConfig::operator=(const LocationConfig& rhs) {
  if (this != &rhs) {
    CommonConfigStore::operator=(rhs);
    route_ = rhs.route_;
  }
}

/*** getter ***/
const std::string& LocationConfig::getRoute() const { return route_; }

/*** setter ***/
void LocationConfig::setRoute(const std::string& route) { route_ = route; }
