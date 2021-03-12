/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonConfigStore.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/10 17:40:46 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/12 19:49:02 by dnakano          ###   ########.fr       */
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
  bool autoIndex_;                                    // autoindex directive
  std::list<std::string> cgi_extension_;              // cgi_extension directive
  std::string charset_;                               // charset directive
  std::string language_;                              // launuage directive
  std::string base_auth_;                             // base_auth directive
  std::list<std::string>
      auth_basic_user_files_;           // auth_basic_userfile_directive
  unsigned long client_max_body_size_;  // client_max_body_size directive
  unsigned long limit_except_;          // allowed functions

 public:
  // coplien
  CommonConfigStore();
  CommonConfigStore(const CommonConfigStore& ref);
  CommonConfigStore& operator=(const CommonConfigStore& ref);
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
  const std::string& getAuthBasicUserFile() const;
  const unsigned long& getClientMaxBodySize() const;
  const unsigned long& getLimitExcept() const;

  /*
  ** parsers
  **
  ** Take content settings as string and then parse it and store the results.
  ** Return values:
  **  0: successfully parsed and stored the result
  **  1: syntax error
  **  2: wrong value was set
  **  3: duplicate value (only for directive cannot be duplicated)
  */

  int parseRoot(const std::string& settings);
  int parseIndex(const std::string& settings);
  int parseErrorPage(const std::string& settings);
  int parseAutoIndex(const std::string& settings);
  int parseCgiExtension(const std::string& settings);
  int parseCharset(const std::string& settings);
  int parseLanguage(const std::string& settings);
  int parseBaseAuth(const std::string& settings);
  int parseAuthBasicUserFile(const std::string& settings);
  int parseClientMaxBodySize(const std::string& settings);
  int parseLimitExcept(const std::string& settings);
};

#endif /* COMMONCONFIGSTORE_HPP */
