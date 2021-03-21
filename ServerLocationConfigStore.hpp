/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerLocationConfigStore.hpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 18:59:06 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/21 09:08:40 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERLOCATIONCONFIGSTORE_HPP
#define SERVERLOCATIONCONFIGSTORE_HPP

#include <list>
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
  ** parsers
  **
  ** Take content settings as string and then parse it and store the results.
  **
  ** These function may throw on error.
  **  - syntax error
  **  - wrong value was set
  **  - duplicate value (only for directive cannot be duplicated)
  */

  void parseUploadPass(const std::list<std::string>& settings);
  void parseUploadStore(const std::list<std::string>& settings);

  virtual bool parseDirective(const std::string& name,
                              const std::list<std::string>& settings);
};

#endif /* SERVERLOCATIONCONFIGSTORE_HPP */
