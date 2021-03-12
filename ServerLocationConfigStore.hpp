/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerLocationConfigStore.hpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 18:59:06 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/12 19:50:24 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERLOCATIONCONFIGSTORE_HPP
#define SERVERLOCATIONCONFIGSTORE_HPP

#include <string>

/*
** ServerLocationConfigStore
**
** Base class of ServerConfig and LocationConfig
** Includes directives which are only avairable in server and location context
*/

class ServerLocationConfigStore {
 protected:
  std::string upload_pass_;
  std::string upload_store_;
 public:
  // coplien
  ServerLocationConfigStore();
  ServerLocationConfigStore(const ServerLocationConfigStore& ref);
  ServerLocationConfigStore& operator=(const ServerLocationConfigStore& ref);
  virtual ~ServerLocationConfigStore();

  // getters
  const std::string& getUploadPass() const;
  const std::string& getUploadStore() const;

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

  int parseUploadPass(const std::string& settings);
  int parseUploadStore(const std::string& settings);
};

#endif /* SERVERLOCATIONCONFIGSTORE_HPP */
