/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonConfigStore.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 18:58:46 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/17 22:07:04 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommonConfigStore.hpp"

CommonConfigStore::CommonConfigStore() {}

CommonConfigStore::CommonConfigStore(const CommonConfigStore& ref) {
  *this = ref;
}

CommonConfigStore& CommonConfigStore::operator=(const CommonConfigStore& rhs) {
  if (this != &rhs) {
    root_ = rhs.root_;
    index_ = rhs.index_;
    error_page_ = rhs.error_page_;
    auto_index_ = rhs.auto_index_;
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

bool CommonConfigStore::getAutoIndex() const { return auto_index_; }

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
