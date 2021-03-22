/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MainOnlyConfigStore.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 18:59:01 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/21 10:14:08 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAINONLYCONFIGSTORE_HPP
#define MAINONLYCONFIGSTORE_HPP

#include <list>
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
  bool flg_max_sessions_set_;   // flag for max_sessions already set
  int retry_after_;   // retry_after directive (times in sec)
  bool flg_retry_after_set_;        // flag for retry_after already set

#ifdef UNIT_TEST
 public:
#endif /* UNIT_TEST */

  /*
  ** persers
  **
  ** Take content settings as string and then parse it and store the results.
  **
  ** These function may throw on error.
  **  - syntax error
  **  - wrong value was set
  **  - duplicate value (only for directive cannot be duplicated)
  */

  void parseMaxSessions(const std::list<std::string>& settings);
  void parseRetryAfter(const std::list<std::string>& settings);

 public:
  // coplien
  MainOnlyConfigStore();
  MainOnlyConfigStore(const MainOnlyConfigStore& ref);
  MainOnlyConfigStore& operator=(const MainOnlyConfigStore& rhs);
  virtual ~MainOnlyConfigStore();

  // getters
  int getMaxSessions() const;
  int getRetryAfter() const;

  virtual bool parseDirective(const std::string& name,
                              const std::list<std::string>& settings);
};

#endif /* MAINONLYCONFIGSTORE_HPP */
