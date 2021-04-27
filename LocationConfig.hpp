/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/01 23:59:30 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/04/27 08:12:50 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONCONFIG_HPP
#define LOCATIONCONFIG_HPP

#include <list>
#include <string>

#include "CommonConfigStore.hpp"
#include "ServerLocationConfigStore.hpp"

class LocationConfig : public CommonConfigStore,
                       public ServerLocationConfigStore {
#ifdef UNIT_TEST
 public:
#else
 private:
#endif
  std::string route_;  // location path

 public:
  LocationConfig();
  LocationConfig(const LocationConfig& ref);
  LocationConfig& operator=(const LocationConfig& rhs);
  virtual ~LocationConfig();

  /*** getter ***/
  const std::string& getRoute() const;

  /*** setter ***/
  void setRoute(const std::string& getRoute);

  bool parseDirective(const std::string& name,
                      const std::list<std::string>& settings);
};

#endif /* LOCATIONCONFIG_HPP */
