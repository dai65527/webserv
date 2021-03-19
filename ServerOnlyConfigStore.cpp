/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerOnlyConfigStore.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 12:09:53 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/19 12:32:24 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerOnlyConfigStore.hpp"

ServerOnlyConfigStore::ServerOnlyConfigStore() {}

ServerOnlyConfigStore::ServerOnlyConfigStore(const ServerOnlyConfigStore& ref) {
  *this = ref;
}

ServerOnlyConfigStore& ServerOnlyConfigStore::operator=(
    const ServerOnlyConfigStore& ref) {
  if (this != &ref) {
    listen_ = ref.listen_;
    server_name_ = ref.server_name_;
  }
  return *this;
}

ServerOnlyConfigStore::~ServerOnlyConfigStore() {}

const std::map<in_addr_t, int>& ServerOnlyConfigStore::getListen() const {
  return listen_;
}

const std::list<std::string>& ServerOnlyConfigStore::getServerName() const {
  return server_name_;
}

void ServerOnlyConfigStore::parseListen(
    const std::list<std::string>& settings) {
  (void)settings;
}

void ServerOnlyConfigStore::parseServerName(
    const std::list<std::string>& settings) {
  if (settings.empty()) {
    throw std::runtime_error("server_name: invalid number of setting");
  }
  server_name_.insert(server_name_.end(), settings.begin(), settings.end());
}
