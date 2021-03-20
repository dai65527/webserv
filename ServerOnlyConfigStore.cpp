/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerOnlyConfigStore.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 12:09:53 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/20 14:59:43 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerOnlyConfigStore.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <cstdlib>

extern "C" {
#include "libft.h"
}

ServerOnlyConfigStore::ServerOnlyConfigStore() {}

ServerOnlyConfigStore::ServerOnlyConfigStore(const ServerOnlyConfigStore& ref) {
  *this = ref;
}

ServerOnlyConfigStore& ServerOnlyConfigStore::operator=(
    const ServerOnlyConfigStore& ref) {
  if (this != &ref) {
    listen_ = ref.listen_;
    server_name_ = ref.server_name_;
  }
  return *this;
}

ServerOnlyConfigStore::~ServerOnlyConfigStore() {}

const std::list<std::pair<in_addr_t, uint16_t> >&
ServerOnlyConfigStore::getListen() const {
  return listen_;
}

const std::list<std::string>& ServerOnlyConfigStore::getServerName() const {
  return server_name_;
}

static void freeStrs(char** strs) {
  for (size_t idx = 0; strs[idx] != 0; idx++) {
    free(strs[idx]);
  }
  free(strs);
}

static size_t countStrs(char** strs) {
  size_t cnt;

  cnt = 0;
  while (strs[cnt] != NULL) {
    cnt++;
  }
  return cnt;
}

// works like strtok(3)
// sep by spaces and returns null when met not only '\0' but also '#'
static char* getNextToken(char* str) {
  static char* pos;  // position saved for next call
  char* start;       // gonna be return val

  if (str != NULL) {
    pos = str;
  }

  // find start
  while (ft_isspace(*pos)) {
    pos++;
  }
  start = pos;

  // find end
  while (*pos != '\0' && *pos != '#' && !ft_isspace(*pos)) {
    pos++;
  }

  // check end
  if (start == pos) {
    return NULL;
  }

  // not to read comment
  if (*pos == '#') {
    *(pos + 1) = '\0';
  } else if (*pos != '\0') {
    *pos = '\0';  // set '\0' to pos
    pos++;        // forward ops for next call
  }

  // return a word
  return start;
}

void ServerOnlyConfigStore::storeEtcHostsToHostIpMap() {
  int fd;
  int ret;
  in_addr_t ip;
  char* line;
  char* token;

  // do nothing if already read
  if (!host_ip_map_.empty()) {
    return;
  }

  // open /etc/hosts (do nothing when failed to open)
  fd = open("/etc/hosts", O_RDONLY);
  if (fd == -1) {
    return;
  }

  // read line and parse
  while (1) {
    ret = get_next_line(fd, &line);
    if (ret == -1) {
      break;
    }

    if (ret == 0 && ft_strlen(line) == 0) {
      break;
    }

    // get IP
    token = getNextToken(line);
    if (token == NULL) {
      free(line);
      continue;
    }
    ip = inet_addr(token);

    // ignore invalid ip
    if (ip == INADDR_NONE) {
      free(line);
      continue;
    }

    // store host name
    while ((token = getNextToken(NULL)) != NULL) {
      host_ip_map_[token] = ip;
    }
    free(line);

    // finish
    if (ret == 0) {
      break;
    }
  }
  close(fd);
}

in_addr_t ServerOnlyConfigStore::getIpFromHostIpMap(const char* str) {
  std::map<std::string, in_addr_t>::const_iterator res;

  storeEtcHostsToHostIpMap();
  res = host_ip_map_.find(str);
  if (res == host_ip_map_.end()) {
    return INADDR_NONE;
  }
  return res->second;
}

in_addr_t ServerOnlyConfigStore::parseIp(const char* str) {
  in_addr_t res;

  if (!ft_strncmp(str, "*", 2)) {
    return INADDR_ANY;
  } else if ((res = inet_addr(str)) != INADDR_NONE) {
    return res;
  } else {
    return getIpFromHostIpMap(str);
  }
}

static bool strIsDigit(const char* str) {
  if (str == NULL || *str == '\0') {
    return false;
  }
  while (*str != '\0') {
    if (!ft_isdigit(*str)) {
      return false;
    }
    str++;
  }
  return true;
}

void ServerOnlyConfigStore::addToListen(const in_addr_t addr,
                                        const uint16_t port) {
  std::list<std::pair<in_addr_t, uint16_t> >::iterator itr;
  for (itr = listen_.begin(); itr != listen_.end(); ++itr) {
    if (itr->second == port) {
      if (itr->first == INADDR_ANY || itr->first == addr) {
        return;
      } else if (addr == INADDR_ANY) {
        itr->first = INADDR_ANY;
        return;
      }
      break;
    }
  }
  listen_.push_back(std::pair<in_addr_t, uint16_t>(addr, port));
}

void ServerOnlyConfigStore::parseListen(
    const std::list<std::string>& settings) {
  if (settings.size() != 1) {
    throw std::runtime_error("listen: invalid number of setting");
  } else if (settings.front().front() == ':' ||
             settings.front().find_last_of(':') ==
                 settings.front().length() - 1) {
    throw std::runtime_error("listen: invalid value \"" + settings.front() +
                             "\"");
  }

  // devide by ':'
  // ex)
  //  "80" => "80"
  //  "example.com" => "example.com"
  //  "123.123.123.123" => "123.123.123.123"
  //  "example.com:80" => "example.com", "80"
  //  "123.123.123.123:80" => "123.123.123.123", "80"
  //  "2001:0db8:1234:5678:90ab:cdef:0000:0000" => many (and will be refused)
  char** ip_port = ft_split(settings.front().c_str(), ':');
  if (ip_port == NULL) {
    throw std::runtime_error("malloc failed");
  }

  size_t cnt = countStrs(ip_port);
  if (cnt == 0 || cnt > 2) {
    freeStrs(ip_port);
    throw std::runtime_error("listen: invalid value \"" + settings.front() +
                             "\"");
  } else if (cnt == 2) {
    const in_addr_t addr = parseIp(ip_port[0]);
    const int port = ft_atoi(ip_port[1]);
    freeStrs(ip_port);
    if (addr == INADDR_NONE) {
      throw std::runtime_error("listen: invalid value \"" + settings.front() +
                               "\"");
    } else if (port < 0 || port > std::numeric_limits<uint16_t>::max()) {
      throw std::runtime_error("listen: invalid port number " +
                               std::to_string(port));
    }
    addToListen(addr, ft_htons(port));
  } else if (cnt == 1 && strIsDigit(ip_port[0])) {
    const int port = ft_atoi(ip_port[0]);
    freeStrs(ip_port);
    if (port < 0 || port > std::numeric_limits<uint16_t>::max()) {
      throw std::runtime_error("listen: invalid port number " +
                               std::to_string(port));
    }
    addToListen(INADDR_ANY, ft_htons(port));
  } else {
    const in_addr_t addr = parseIp(ip_port[0]);
    freeStrs(ip_port);
    if (addr == INADDR_NONE) {
      throw std::runtime_error("listen: invalid value \"" + settings.front() +
                               "\"");
    }
    addToListen(addr, ft_htons(80));
  }
}

void ServerOnlyConfigStore::parseServerName(
    const std::list<std::string>& settings) {
  if (settings.empty()) {
    throw std::runtime_error("server_name: invalid number of setting");
  }
  server_name_.insert(server_name_.end(), settings.begin(), settings.end());
}
