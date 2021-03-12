/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MainOnlyConfigStore.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 18:59:01 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/12 19:53:29 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAINONLYCONFIGSTORE_HPP
#define MAINONLYCONFIGSTORE_HPP

#include <string>

/*
** ServerLocationConfigStore
**
** Base class of MainConfig
** Includes directives which are only avairable in main context
*/

class MainOnlyConfigStore {
 protected:
  int max_sessions_;  // max_sessions directive
  int retry_after_;   // retry_after directive (times in sec)

 public:
  // coplien
  MainOnlyConfigStore();
  MainOnlyConfigStore(const MainOnlyConfigStore& ref);
  MainOnlyConfigStore& operator=(const MainOnlyConfigStore& ref);
  virtual ~MainOnlyConfigStore();

  // getters
  int getMaxSessions() const;
  int getRetryAfter() const;

  /*
  ** persers
  **
  ** Take content settings as string and then parse it and store the results.
  ** Return values:
  **  0: successfully parsed and stored the result
  **  1: syntax error
  **  2: wrong value was set
  **  3: duplicate value (only for directive cannot be duplicated)
  */

  int parseMaxSessions(const std::string& settings);
  int parseRetryAfter(const std::string& settings);
};

#endif /* MAINONLYCONFIGSTORE_HPP */
