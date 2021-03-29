/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MainConfig.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 18:58:53 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/28 12:17:06 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAINCONFIG_HPP
#define MAINCONFIG_HPP

#include <list>
#include <string>

#include "CommonConfigStore.hpp"
#include "MainOnlyConfigStore.hpp"
#include "ServerConfig.hpp"

class MainConfig : public CommonConfigStore, public MainOnlyConfigStore {
 private:
#ifdef UNIT_TEST
 public:
#endif
  std::list<ServerConfig> servers_;

 public:
  MainConfig();
  MainConfig(const MainConfig& ref);
  virtual ~MainConfig();
  MainConfig& operator=(const MainConfig& rhs);

  const std::list<ServerConfig>& getServers() const;
  void addServer(const ServerConfig& server);
  bool parseDirective(const std::string& name,
                      const std::list<std::string>& settings);
};

#endif /* MAINCONFIG_HPP */
