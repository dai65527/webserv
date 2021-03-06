/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonConfigStore.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 18:58:46 by dnakano           #+#    #+#             */
/*   Updated: 2021/05/06 11:15:36 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommonConfigStore.hpp"

#include "webserv_utils.hpp"

extern "C" {
#include "libft.h"
}

CommonConfigStore::CommonConfigStore() {
  autoindex_ = false;
  flg_autoindex_set_ = false;
  client_max_body_size_ = 1000000;
  flg_client_max_body_size_set_ = false;
  limit_except_ = HTTP_GET | HTTP_HEAD;
  flg_limit_except_set_ = false;
}

CommonConfigStore::CommonConfigStore(const CommonConfigStore& ref) {
  *this = ref;
}

CommonConfigStore& CommonConfigStore::operator=(const CommonConfigStore& rhs) {
  if (this != &rhs) {
    root_ = rhs.root_;
    index_ = rhs.index_;
    error_page_ = rhs.error_page_;
    autoindex_ = rhs.autoindex_;
    flg_autoindex_set_ = rhs.flg_autoindex_set_;
    cgi_extension_ = rhs.cgi_extension_;
    cgi_pass_ = rhs.cgi_pass_;
    charset_ = rhs.charset_;
    language_ = rhs.language_;
    auth_basic_user_pass_ = rhs.auth_basic_user_pass_;
    client_max_body_size_ = rhs.client_max_body_size_;
    flg_client_max_body_size_set_ = rhs.flg_client_max_body_size_set_;
    limit_except_ = rhs.limit_except_;
  }
  return *this;
}

CommonConfigStore::~CommonConfigStore() {}

/*** getters ***/
const std::string& CommonConfigStore::getRoot() const { return root_; }

const std::list<std::string>& CommonConfigStore::getIndex() const {
  return index_;
}

const std::map<HTTPStatusCode, std::string>& CommonConfigStore::getErrorPage()
    const {
  return error_page_;
}

bool CommonConfigStore::getAutoIndex() const { return autoindex_; }

const std::list<std::string>& CommonConfigStore::getCgiExtension() const {
  return cgi_extension_;
}

const std::string& CommonConfigStore::getCgiPass() const {
  return cgi_pass_;
}

const std::string& CommonConfigStore::getCharset() const { return charset_; }

const std::list<std::string>& CommonConfigStore::getLanguage() const {
  return language_;
}

const std::list<std::string>& CommonConfigStore::getAuthBasicUserPass() const {
  return auth_basic_user_pass_;
}

const unsigned long& CommonConfigStore::getClientMaxBodySize() const {
  return client_max_body_size_;
}

const unsigned long& CommonConfigStore::getLimitExcept() const {
  return limit_except_;
}

bool CommonConfigStore::getFlgClientMaxBodySizeSet() const {
  return flg_client_max_body_size_set_;
}

/*** parsers ***/
void CommonConfigStore::parseRoot(const std::list<std::string>& settings) {
  if (!root_.empty()) {
    throw std::runtime_error("root: directive duplicated");
  } else if (settings.size() != 1) {
    throw std::runtime_error("root: invalid number of setting");
  }
  root_ = settings.front();
}

void CommonConfigStore::parseIndex(const std::list<std::string>& settings) {
  if (settings.empty()) {
    throw std::runtime_error("index: invalid number of setting");
  }
  index_.insert(index_.end(), settings.begin(), settings.end());
}

void CommonConfigStore::parseErrorPage(const std::list<std::string>& settings) {
  HTTPStatusCode code;

  // check number of settings
  if (settings.size() < 2) {
    throw std::runtime_error("error_page: invalid number of setting");
  }

  // set all settings
  std::list<std::string>::const_iterator itr;
  std::list<std::string>::const_iterator itr_end = --settings.end();
  for (itr = settings.begin(); itr != itr_end; ++itr) {
    // check code is valid (must be between 300 and 599 because it's an "error")
    code = isHttpStatusCode(atoi(itr->c_str()));
    if (code == HTTP_NOMATCH) {
      throw std::runtime_error("error_page: invalid value \"" + *itr + "\"");
    } else if (code < 300) {
      throw std::runtime_error("error_page: value must be between 300 and 599");
    }
    error_page_[code] = settings.back();  // settings.back();
  }
}

void CommonConfigStore::parseAutoIndex(const std::list<std::string>& settings) {
  if (flg_autoindex_set_) {
    throw std::runtime_error("autoindex: directive duplicated");
  } else if (settings.size() != 1) {
    throw std::runtime_error("autoindex: invalid number of setting");
  }

  if (!settings.front().compare("on")) {
    autoindex_ = true;
  } else if (!settings.front().compare("off")) {
    autoindex_ = false;
  } else {
    throw std::runtime_error("autoindex: invalid value \"" + settings.front() +
                             "\"");
  }
  flg_autoindex_set_ = true;
}

void CommonConfigStore::parseCgiExtension(
    const std::list<std::string>& settings) {
  if (settings.empty()) {
    throw std::runtime_error("cgi_extension: invalid number of setting");
  }
  cgi_extension_.insert(cgi_extension_.end(), settings.begin(), settings.end());
}

void CommonConfigStore::parseCgiPass(const std::list<std::string>& settings) {
  if (!cgi_pass_.empty()) {
    throw std::runtime_error("cgi_pass: directive duplicated");
  } else if (settings.size() != 1) {
    throw std::runtime_error("cgi_pass: invalid number of setting");
  }
  cgi_pass_ = settings.front();
}

void CommonConfigStore::parseCharset(const std::list<std::string>& settings) {
  if (!charset_.empty()) {
    throw std::runtime_error("charset: directive duplicated");
  } else if (settings.size() != 1) {
    throw std::runtime_error("charset: invalid number of setting");
  }
  charset_ = settings.front();
}

void CommonConfigStore::parseLanguage(const std::list<std::string>& settings) {
  if (!language_.empty()) {
    throw std::runtime_error("language: directive duplicated");
  } else if (settings.empty()) {
    throw std::runtime_error("language: invalid number of setting");
  }
  // check and store language name
  for (std::list<std::string>::const_iterator itr = settings.begin();
       itr != settings.end(); ++itr) {
    language_.push_back(*itr);
  }
}

// check userpass is like `username:password`
static bool isValidUserPass(const std::string& userpass) {
  size_t pos_first = userpass.find(':');
  size_t pos_last = userpass.find_last_of(':');

  if (pos_first == std::string::npos) {
    return false;  // no ':' (ex: `user`)
  } else if (pos_first != pos_last) {
    return false;  // dup ':' (ex: `user:pass:hoge`)
  } else if (pos_first == 0) {
    return false;  // no user name (ex: `:pass`) (allow no pass)
  }
  return true;
}

void CommonConfigStore::parseAuthBasicUserPass(
    const std::list<std::string>& settings) {
  if (settings.empty()) {
    throw std::runtime_error("auth_basic_user_pass: invalid number of setting");
  }
  for (std::list<std::string>::const_iterator itr = settings.begin();
       itr != settings.end(); ++itr) {
    if (!isValidUserPass(*itr)) {
      throw std::runtime_error("auth_basic_user_pass: invalid value \"" + *itr +
                               "\"");
    }
  }
  auth_basic_user_pass_.insert(auth_basic_user_pass_.end(), settings.begin(),
                               settings.end());
}

void CommonConfigStore::parseClientMaxBodySize(
    const std::list<std::string>& settings) {
  if (flg_client_max_body_size_set_) {
    throw std::runtime_error("client_max_body_size: directive duplicated");
  } else if (settings.size() != 1) {
    throw std::runtime_error("client_max_body_size: invalid number of setting");
  }

  client_max_body_size_ = 1;

  // check value of setting
  for (const char* value_str = settings.front().c_str(); *value_str != '\0';
       value_str++) {
    if (!isdigit(*value_str)) {
      if ((*value_str == 'k' || *value_str == 'K') &&
          *(value_str + 1) == '\0' && settings.front().length() != 1) {
        client_max_body_size_ = 1000;
        break;
      } else if ((*value_str == 'm' || *value_str == 'M') &&
                 *(value_str + 1) == '\0' && settings.front().length() != 1) {
        client_max_body_size_ = 1000000;
        break;
      } else if ((*value_str == 'g' || *value_str == 'G') &&
                 *(value_str + 1) == '\0' && settings.front().length() != 1) {
        client_max_body_size_ = 1000000000;
        break;
      }
      throw std::runtime_error("client_max_body_size: invalid value \"" +
                               settings.front() + "\"");
    }
  }

  client_max_body_size_ *= ft_atoul(settings.front().c_str());
  flg_client_max_body_size_set_ = true;
}

void CommonConfigStore::parseLimitExcept(
    const std::list<std::string>& settings) {
  if (flg_limit_except_set_) {
    throw std::runtime_error("limit_except: directive duplicated");
  } else if (settings.empty()) {
    throw std::runtime_error("limit_except: invalid number of setting");
  }

  limit_except_ = 0;  // delete default value
  for (std::list<std::string>::const_iterator itr = settings.begin();
       itr != settings.end(); ++itr) {
    if (*itr == "GET") {
      // limit_except_ |= (HTTP_GET | HTTP_HEAD);
      limit_except_ |= HTTP_GET;
    } else if (*itr == "HEAD") {
      limit_except_ |= HTTP_HEAD;
    } else if (*itr == "PUT") {
      limit_except_ |= HTTP_PUT;
    } else if (*itr == "POST") {
      limit_except_ |= HTTP_POST;
    } else if (*itr == "DELETE") {
      limit_except_ |= HTTP_DELETE;
    } else if (*itr == "CONNECT") {
      throw std::runtime_error("limit_except: CONNECT method not supported");
    } else if (*itr == "OPTIONS") {
      limit_except_ |= HTTP_OPTIONS;
    } else if (*itr == "TRACE") {
      limit_except_ |= HTTP_TRACE;
    } else {
      throw std::runtime_error("limit_except: invalid value \"" + *itr + "\"");
    }
  }
  flg_limit_except_set_ = true;
}

/*
** parseDirective
**
** return true if stored
** false otherwise
*/

bool CommonConfigStore::parseDirective(const std::string& name,
                                       const std::list<std::string>& settings) {
  if (name == "root") {
    parseRoot(settings);
  } else if (name == "index") {
    parseIndex(settings);
  } else if (name == "error_page") {
    parseErrorPage(settings);
  } else if (name == "autoindex") {
    parseAutoIndex(settings);
  } else if (name == "cgi_extension") {
    parseCgiExtension(settings);
  } else if (name == "cgi_pass") {
    parseCgiPass(settings);
  } else if (name == "charset") {
    parseCharset(settings);
  } else if (name == "language") {
    parseLanguage(settings);
  } else if (name == "auth_basic_user_pass") {
    parseAuthBasicUserPass(settings);
  } else if (name == "client_max_body_size") {
    parseClientMaxBodySize(settings);
  } else if (name == "limit_except") {
    parseLimitExcept(settings);
  } else {
    return false;
  }
  return true;
}
