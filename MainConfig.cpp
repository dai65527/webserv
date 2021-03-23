/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MainConfig.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 18:34:15 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/23 19:00:34 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MainConfig.hpp"

MainConfig::MainConfig() {}

MainConfig::MainConfig(const MainConfig& ref) { *this = ref; }

MainConfig::~MainConfig() {}

MainConfig& MainConfig::operator=(const MainConfig& rhs) {
  if (this != &rhs) {
    CommonConfigStore::operator=(rhs);
    MainOnlyConfigStore::operator=(rhs);
    servers_ = rhs.servers_;
  }
  return *this;
}

const std::list<ServerConfig>& MainConfig::getServers() const {
  return servers_;
}

void MainConfig::addServer(const ServerConfig& server) {
  servers_.push_back(server);
}

bool MainConfig::parseDirective(const std::string& name,
                                const std::list<std::string>& settings) {
  if (!MainOnlyConfigStore::parseDirective(name, settings) &&
      !CommonConfigStore::parseDirective(name, settings)) {
    throw std::runtime_error(name + ": invalid directive name in main context");
  }
  return true;
}
