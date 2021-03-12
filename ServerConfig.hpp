/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 18:58:48 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/12 19:50:29 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <list>
#include <string>

#include "CommonConfigStore.hpp"
#include "ServerLocationConfigStore.hpp"
#include "ServerOnlyConfigStore.hpp"
#include "LocationConfig.hpp"

class ServerConfig : public CommonConfigStore,
                     public ServerLocationConfigStore,
                     public ServerOnlyConfigStore {
 private:
  std::list<LocationConfig> location_config_list_;

 public:
  ServerConfig();
  ServerConfig(const ServerConfig& ref);
  ServerConfig& operator=(const ServerConfig& rhs);
  virtual ~ServerConfig();

  // int load(type data);
};

#endif /* SERVERCONFIG_HPP */
