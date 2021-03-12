/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MainConfig.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 18:58:53 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/12 19:53:16 by dnakano          ###   ########.fr       */
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
  std::string location;  // location path
  std::list<ServerConfig> servers_;

 public:
  MainConfig();
  MainConfig(const MainConfig& ref);
  MainConfig& operator=(const MainConfig& rhs);
  virtual ~MainConfig();

  /*
  ** loadConfigFile
  **
  ** Read and parse a config file.
  ** Return number of error in config file.
  */

  int loadConfigFile(const std::string& filename);

  // TODO: find config
};

#endif /* MAINCONFIG_HPP */
