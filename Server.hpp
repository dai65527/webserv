/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/01 23:36:35 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/23 18:31:31 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/socket.h>

#include <map>
#include <string>

#include "LocationConfig.hpp"

class Server {
 private:
  std::map<int, in_addr_t> port_ip_;
  std::list<std::string> names_;
  size_t max_client_body_size_;
  LocationConfig default_location_config_;
  std::list<LocationConfig> location_configs_;

 public:
  Server();
  virtual ~Server();
  Server(const Server& other);
  Server& operator=(const Server& other);

  const std::map<int, in_addr_t>& getPortIp() const;
  const std::list<std::string>& getNames() const;
  size_t getMaxClientBodySize() const;
  const LocationConfig& getDefaultLocationConfig() const;
  int addPortIpToMap(int port, std::string ip);
  int addServerName(const std::list<std::string>& names);
  void setDefaultLocationConfig(const LocationConfig& default_location_config);
  int addToLocationConfig(const LocationConfig& location_config);
  int configIsValid() const;
  int isMatch(int port, in_addr_t ip, std::string server_name) const;
};

#endif /* SERVER_HPP */
