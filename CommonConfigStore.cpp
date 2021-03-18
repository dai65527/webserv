/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonConfigStore.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 18:58:46 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/18 14:03:18 by dnakano          ###   ########.fr       */
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
    charset_ = rhs.charset_;
    language_ = rhs.language_;
    base_auth_ = rhs.base_auth_;
    auth_basic_user_file_ = rhs.auth_basic_user_file_;
    client_max_body_size_ = rhs.client_max_body_size_;
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

const std::string& CommonConfigStore::getCharset() const { return charset_; }

const std::string& CommonConfigStore::getLanguage() const { return language_; }

const std::string& CommonConfigStore::getBaseAuth() const { return base_auth_; }

const std::list<std::string>& CommonConfigStore::getAuthBasicUserFile() const {
  return auth_basic_user_file_;
}

const unsigned long& CommonConfigStore::getClientMaxBodySize() const {
  return client_max_body_size_;
}

const unsigned long& CommonConfigStore::getLimitExcept() const {
  return limit_except_;
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
    code = isHttpStatusCode(ft_atoi(itr->c_str()));
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

void CommonConfigStore::parseCharset(const std::list<std::string>& settings) {
  if (!charset_.empty()) {
    throw std::runtime_error("charset: directive duplicated");
  } else if (settings.size() != 1) {
    throw std::runtime_error("charset: invalid number of setting");
  }
  // need to check charset name???
  charset_ = settings.front();
}

void CommonConfigStore::parseLanguage(const std::list<std::string>& settings) {
  if (!language_.empty()) {
    throw std::runtime_error("language: directive duplicated");
  } else if (settings.size() != 1) {
    throw std::runtime_error("language: invalid number of setting");
  }
  // need to check language name???
  language_ = settings.front();
}

void CommonConfigStore::parseBaseAuth(const std::list<std::string>& settings) {
  if (!base_auth_.empty()) {
    throw std::runtime_error("base_auth: directive duplicated");
  } else if (settings.size() != 1) {
    throw std::runtime_error("base_auth: invalid number of setting");
  }
  // need to check language name???
  base_auth_ = settings.front();
}
