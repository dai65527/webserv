/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerLocationConfigStore.cpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 11:31:25 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/19 11:45:14 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerLocationConfigStore.hpp"

#include <list>

ServerLocationConfigStore::ServerLocationConfigStore() {}

ServerLocationConfigStore::ServerLocationConfigStore(
    const ServerLocationConfigStore& ref) {
  *this = ref;
}

ServerLocationConfigStore& ServerLocationConfigStore::operator=(
    const ServerLocationConfigStore& ref) {
  if (this != &ref) {
    upload_pass_ = ref.upload_pass_;
    upload_store_ = ref.upload_store_;
  }
  return *this;
}

ServerLocationConfigStore::~ServerLocationConfigStore() {}

/*** getters ***/
const std::string& ServerLocationConfigStore::getUploadPass() const {
  return upload_pass_;
}

const std::string& ServerLocationConfigStore::getUploadStore() const {
  return upload_store_;
}

/*** parsers ***/
void ServerLocationConfigStore::parseUploadPass(
    const std::list<std::string>& settings) {
  if (!upload_pass_.empty()) {
    throw std::runtime_error("upload_pass: directive duplicated");
  } else if (settings.size() != 1) {
    throw std::runtime_error("upload_pass: invalid number of setting");
  }
  upload_pass_ = settings.front();
}

// void ServerLocationConfigStore::parseUploadStore(const std::string&
// settings);
