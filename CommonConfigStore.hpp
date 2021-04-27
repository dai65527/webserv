/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonConfigStore.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/10 17:40:46 by dnakano           #+#    #+#             */
/*   Updated: 2021/04/27 01:04:15 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMONCONFIGSTORE_HPP
#define COMMONCONFIGSTORE_HPP

#include <list>
#include <map>
#include <string>

#include "HttpMethodFlag.hpp"
#include "HttpStatusCode.hpp"

/*
** CommonConfigStore
**
** Base class of MainConfig, ServerConfig and LocationConfig
*/

class CommonConfigStore {
 protected:
#ifdef UNIT_TEST
 public:
#endif /* UNIT_TEST */

  std::string root_;                                  // root directive
  std::list<std::string> index_;                      // index directive
  std::map<HTTPStatusCode, std::string> error_page_;  // error_page directive
  bool autoindex_;                                    // autoindex directive
  bool flg_autoindex_set_;                // true if autoindex already set
  std::list<std::string> cgi_extension_;  // cgi_extension directive
  std::string charset_;                   // charset directive
  std::list<std::string> language_;       // launuage directive
  std::string base_auth_;                 // base_auth directive
  std::list<std::string>
      auth_basic_user_file_;            // auth_basic_userfile directive
  unsigned long client_max_body_size_;  // client_max_body_size directive
  bool flg_client_max_body_size_set_;   // true if CMBS already set
  unsigned long limit_except_;          // allowed functions
  bool flg_limit_except_set_;           // true if limit_except already set

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
  const std::list<std::string>& getLanguage() const;
  const std::string& getBaseAuth() const;
  const std::list<std::string>& getAuthBasicUserFile() const;
  const unsigned long& getClientMaxBodySize() const;
  const unsigned long& getLimitExcept() const;
  bool getFlgClientMaxBodySizeSet() const;

  /*
  ** store_config
  */
  virtual bool parseDirective(const std::string& name,
                              const std::list<std::string>& settings);
};

#endif /* COMMONCONFIGSTORE_HPP */
