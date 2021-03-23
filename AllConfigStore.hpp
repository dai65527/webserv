/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AllConfigStore.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 19:59:58 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/12 22:54:50 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALLCONFIGSTORE_HPP
#define ALLCONFIGSTORE_HPP

#include "CommonConfigStore.hpp"
#include "MainConfig.hpp"
#include "MainOnlyConfigStore.hpp"
#include "ServerLocationConfigStore.hpp"
#include "ServerOnlyConfigStore.hpp"

/*
** AllConfigStore
**
** This has all directives!!
*/

class AllConfigStore : public CommonConfigStore,
                       public MainOnlyConfigStore,
                       public ServerOnlyConfigStore,
                       public ServerLocationConfigStore {
 public:
  AllConfigStore();
  AllConfigStore(const AllConfigStore& ref);
  virtual ~AllConfigStore();
  AllConfigStore& operator=(const AllConfigStore& rhs);

  void importConfig(const std::string& route, const in_addr_t& ip, int port,
                    const MainConfig& main_confg);
};

#endif /* ALLCONFIGSTORE_HPP */
