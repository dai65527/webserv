/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonConfigStore.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/10 17:40:46 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/17 19:39:45 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMONCONFIGSTORE_HPP
#define COMMONCONFIGSTORE_HPP

#include <list>
#include <map>
#include <string>

#include "HttpStatusCode.hpp"

/*
** CommonConfigStore
**
** Base class of MainConfig, ServerConfig and LocationConfig
*/

class CommonConfigStore {
 protected:
  std::string root_;                                  // root directive
  std::list<std::string> index_;                      // index directive
  std::map<HTTPStatusCode, std::string> error_page_;  // error_page directive
  bool auto_index_;                                   // autoindex directive
  std::list<std::string> cgi_extension_;              // cgi_extension directive
  std::string charset_;                               // charset directive
  std::string language_;                              // launuage directive
  std::string base_auth_;                             // base_auth directive
  std::list<std::string>
      auth_basic_user_file_;            // auth_basic_userfile directive
  unsigned long client_max_body_size_;  // client_max_body_size directive
  unsigned long limit_except_;          // allowed functions

 public:
  // coplien
  CommonConfigStore();
  CommonConfigStore(const CommonConfigStore& ref);
  CommonConfigStore& operator=(const CommonConfigStore& rhs);
  virtual ~CommonConfigStore();

  // getters
  const std::string& getRoot() const;
  const std::list<std::string>& getIndex() const;
  const std::map<HTTPStatusCode, std::string>& getErrorPage() const;
  bool getAutoIndex() const;
  const std::list<std::string>& getCgiExtension() const;
  const std::string& getCharset() const;
  const std::string& getLanguage() const;
  const std::string& getBaseAuth() const;
  const std::list<std::string>& getAuthBasicUserFile() const;
  const unsigned long& getClientMaxBodySize() const;
  const unsigned long& getLimitExcept() const;

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

  void parseRoot(const std::list<std::string>& settings);
  void parseIndex(const std::list<std::string>& settings);
  void parseErrorPage(const std::list<std::string>& settings);
  void parseAutoIndex(const std::list<std::string>& settings);
  void parseCgiExtension(const std::list<std::string>& settings);
  void parseCharset(const std::list<std::string>& settings);
  void parseLanguage(const std::list<std::string>& settings);
  void parseBaseAuth(const std::list<std::string>& settings);
  void parseAuthBasicUserFile(const std::list<std::string>& settings);
  void parseClientMaxBodySize(const std::list<std::string>& settings);
  void parseLimitExcept(const std::list<std::string>& settings);
};

#endif /* COMMONCONFIGSTORE_HPP */