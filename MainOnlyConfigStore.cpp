/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MainOnlyConfigStore.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 15:05:16 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/21 10:15:43 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MainOnlyConfigStore.hpp"

extern "C" {
#include "libft.h"
}

// coplien
MainOnlyConfigStore::MainOnlyConfigStore() {
  max_sessions_ = 1000;
  retry_after_ = 120;
  flg_max_sessions_set_ = false;
  flg_retry_after_set_ = false;
}

MainOnlyConfigStore::MainOnlyConfigStore(const MainOnlyConfigStore& ref) {
  *this = ref;
}

MainOnlyConfigStore& MainOnlyConfigStore::MainOnlyConfigStore::operator=(
    const MainOnlyConfigStore& rhs) {
  if (this != &rhs) {
    max_sessions_ = rhs.max_sessions_;
    retry_after_ = rhs.retry_after_;
    flg_max_sessions_set_ = rhs.flg_max_sessions_set_;
    flg_retry_after_set_ = rhs.flg_retry_after_set_;
  }
  return *this;
}

MainOnlyConfigStore::~MainOnlyConfigStore() {}

/*** getters ***/
int MainOnlyConfigStore::getMaxSessions() const { return max_sessions_; }
int MainOnlyConfigStore::getRetryAfter() const { return retry_after_; }

/*** parsers helper ***/
static bool str_isdigit(const char* str) {
  while (*str != '\0') {
    if (!ft_isdigit(*str)) {
      return false;
    }
    str++;
  }
  return true;
}

/*** parsers ***/
void MainOnlyConfigStore::parseMaxSessions(
    const std::list<std::string>& settings) {
  if (flg_max_sessions_set_) {
    throw std::runtime_error("max_sessions: directive duplicated");
  } else if (settings.size() != 1) {
    throw std::runtime_error("max_sessions: invalid number of setting");
  } else if (!str_isdigit(settings.front().c_str()) ||
             settings.front().length() > 10) {
    throw std::runtime_error("max_sessions: invalid value \"" +
                             settings.front() + "\"");
  }

  unsigned long val = ft_atoul(settings.front().c_str());
  if (val > static_cast<unsigned long>(std::numeric_limits<int>::max())) {
    throw std::runtime_error("max_sessions: invalid value \"" +
                             settings.front() + "\"");
  }

  max_sessions_ = ft_atoi(settings.front().c_str());
  if (max_sessions_ < 1) {
    throw std::runtime_error("max_sessions: invalid value \"" +
                             settings.front() + "\"");
  }
  flg_max_sessions_set_ = true;
}

void MainOnlyConfigStore::parseRetryAfter(
    const std::list<std::string>& settings) {
  if (flg_retry_after_set_) {
    throw std::runtime_error("retry_after: directive duplicated");
  } else if (settings.size() != 1) {
    throw std::runtime_error("retry_after: invalid number of setting");
  } else if (!str_isdigit(settings.front().c_str()) ||
             settings.front().length() > 10) {
    throw std::runtime_error("retry_after: invalid value \"" +
                             settings.front() + "\"");
  }

  unsigned long val = ft_atoul(settings.front().c_str());
  if (val > static_cast<unsigned long>(std::numeric_limits<int>::max())) {
    throw std::runtime_error("retry_after: invalid value \"" +
                             settings.front() + "\"");
  }

  retry_after_ = ft_atoi(settings.front().c_str());
  if (retry_after_ < 1) {
    throw std::runtime_error("retry_after: invalid value \"" +
                             settings.front() + "\"");
  }
  flg_retry_after_set_ = true;
}

/*
** parseDirective
**
** return true if stored
** false otherwise
*/

bool MainOnlyConfigStore::parseDirective(
    const std::string& name, const std::list<std::string>& settings) {
  if (name == "max_sessions") {
    parseMaxSessions(settings);
  } else if (name == "retry_after") {
    parseRetryAfter(settings);
  } else {
    return false;
  }
  return true;
}
