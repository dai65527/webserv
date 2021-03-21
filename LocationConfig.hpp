/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/01 23:59:30 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/21 11:38:52 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONCONFIG_HPP
#define LOCATIONCONFIG_HPP

#include <list>
#include <string>

#include "CommonConfigStore.hpp"
#include "MainOnlyConfigStore.hpp"

class LocationConfig : public CommonConfigStore {
 private:
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
};

#endif /* LOCATIONCONFIG_HPP */
