/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/01 23:59:30 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/12 19:53:15 by dnakano          ###   ########.fr       */
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
  std::string location_;  // location path

 public:
  LocationConfig();
  LocationConfig(const LocationConfig& ref);
  LocationConfig& operator=(const LocationConfig& rhs);
  virtual ~LocationConfig();

  // int load(type data);
};

#endif /* LOCATIONCONFIG_HPP */
