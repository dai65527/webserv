/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv_utils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 08:14:01 by dnakano           #+#    #+#             */
/*   Updated: 2021/05/06 10:57:45 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv_utils.hpp"

#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>

#include <ctime>
#include <string>

#include "HttpStatusCode.hpp"

extern "C" {
#include "libft.h"
}

/*
** isHttpStatusCode
**
** Check if code is HTTPStatusCode or not.
** If so, return the code, otherwise returns -1
*/

HTTPStatusCode isHttpStatusCode(int code) {
  switch (code) {
    case HTTP_100:
      return HTTP_100;
    case HTTP_101:
      return HTTP_101;
    case HTTP_102:
      return HTTP_102;
    case HTTP_103:
      return HTTP_103;
    case HTTP_200:
      return HTTP_200;
    case HTTP_201:
      return HTTP_201;
    case HTTP_202:
      return HTTP_202;
    case HTTP_203:
      return HTTP_203;
    case HTTP_204:
      return HTTP_204;
    case HTTP_205:
      return HTTP_205;
    case HTTP_206:
      return HTTP_206;
    case HTTP_207:
      return HTTP_207;
    case HTTP_208:
      return HTTP_208;
    case HTTP_226:
      return HTTP_226;
    case HTTP_300:
      return HTTP_300;
    case HTTP_301:
      return HTTP_301;
    case HTTP_302:
      return HTTP_302;
    case HTTP_303:
      return HTTP_303;
    case HTTP_304:
      return HTTP_304;
    case HTTP_305:
      return HTTP_305;
    case HTTP_306:
      return HTTP_306;
    case HTTP_307:
      return HTTP_307;
    case HTTP_308:
      return HTTP_308;
    case HTTP_400:
      return HTTP_400;
    case HTTP_401:
      return HTTP_401;
    case HTTP_403:
      return HTTP_403;
    case HTTP_404:
      return HTTP_404;
    case HTTP_405:
      return HTTP_405;
    case HTTP_406:
      return HTTP_406;
    case HTTP_407:
      return HTTP_407;
    case HTTP_408:
      return HTTP_408;
    case HTTP_409:
      return HTTP_409;
    case HTTP_410:
      return HTTP_410;
    case HTTP_411:
      return HTTP_411;
    case HTTP_412:
      return HTTP_412;
    case HTTP_413:
      return HTTP_413;
    case HTTP_414:
      return HTTP_414;
    case HTTP_415:
      return HTTP_415;
    case HTTP_416:
      return HTTP_416;
    case HTTP_417:
      return HTTP_417;
    case HTTP_418:
      return HTTP_418;
    case HTTP_421:
      return HTTP_421;
    case HTTP_422:
      return HTTP_422;
    case HTTP_423:
      return HTTP_423;
    case HTTP_424:
      return HTTP_424;
    case HTTP_425:
      return HTTP_425;
    case HTTP_426:
      return HTTP_426;
    case HTTP_428:
      return HTTP_428;
    case HTTP_429:
      return HTTP_429;
    case HTTP_431:
      return HTTP_431;
    case HTTP_451:
      return HTTP_451;
    case HTTP_500:
      return HTTP_500;
    case HTTP_501:
      return HTTP_501;
    case HTTP_502:
      return HTTP_502;
    case HTTP_503:
      return HTTP_503;
    case HTTP_504:
      return HTTP_504;
    case HTTP_505:
      return HTTP_505;
    case HTTP_506:
      return HTTP_506;
    case HTTP_507:
      return HTTP_507;
    case HTTP_508:
      return HTTP_508;
    case HTTP_510:
      return HTTP_510;
    case HTTP_511:
      return HTTP_511;
    default:
      return HTTP_NOMATCH;
  }
}

bool isDirectory(const std::string& path) {
  struct stat pathstat;

  if (stat(path.c_str(), &pathstat) == 0) {
    return false;
  }
  return S_ISDIR(pathstat.st_mode);
}

/*
** getTimeStamp
**
** Obtain time stamp of time when function called and store to buf (bufsize).
** The timestamp will follow `fmt' aruguments. (refer to `man strftime' for
** detail)
*/

size_t getTimeStamp(char* buf, size_t bufsize, const char* fmt,
                    time_t unixtime) {
  // convert char string to struct tm
  struct tm* time;
  time = gmtime(&unixtime);
  strncpy(time->tm_zone, "GMT",
          strlen(time->tm_zone));  // UTC->GMT (almost same)

  // convert tm to string
  return strftime(buf, bufsize, fmt, time);
}

size_t getTimeStamp(char* buf, size_t bufsize, const char* fmt) {
  // get time
  struct timeval tv;
  if (gettimeofday(&tv, NULL) == -1) {
    buf[0] = '\0';
    return 0;
  }

  return getTimeStamp(buf, bufsize, fmt, tv.tv_sec);
}

std::string extension(const std::string& filename) {
  // find dot
  size_t pos_dot = filename.find_last_of('.');
  if (pos_dot == std::string::npos || pos_dot == 0) {
    return "";
  }

  // find slash to eliminate case like "/dir/.hoge" or "/.dir/filename"
  size_t pos_slash = filename.find_last_of('/');
  if (pos_slash != std::string::npos && pos_slash + 1 >= pos_dot) {
    return "";
  }

  std::string extension = filename.substr(pos_dot + 1);
  for (std::string::iterator itr = extension.begin(); itr != extension.end();
       ++itr) {
    *itr = tolower(*itr);
  }
  return extension;
}

std::string basename(const std::string& path) {
  if (path.empty()) {
    return "";
  }

  size_t end = path.find_last_not_of('/');
  if (end == std::string::npos) {
    return "/";
  }

  size_t begin = path.find_last_of('/', end);
  if (begin == std::string::npos) {
    return path.substr(0, end + 1);
  }
  return path.substr(begin + 1, end - begin);
}

/*
** isLitteleEndian
** find Little or Big endian of the environment
*/

int isLitteleEndian() {
  uint16_t n;
  uint8_t* uc;

  n = 1;
  uc = (uint8_t*)&n;
  return (*uc);
}

/*
** getIpAddress
** convert uint32_t to 192.168.0.1 style string
*/

std::string getIpAddress(uint32_t ip) {
  uint8_t* ip_array;
  std::string ret;
  char* tmp;

  ip_array = reinterpret_cast<uint8_t*>(&ip);
  if (isLitteleEndian()) {
    for (int i = 3; i > -1; --i) {
      if (i != 3) {
        ret.insert(0, ".");
      }
      tmp = ft_itoa(ip_array[i]);
      if (!tmp) {
        throw std::bad_alloc();
      }
      ret.insert(0, tmp);
      free(tmp);
    }
  } else {
    for (int i = 0; i < 4; ++i) {
      if (i != 0) {
        ret.insert(0, ".");
      }
      tmp = ft_itoa(ip_array[i]);
      if (!tmp) {
        throw std::bad_alloc();
      }
      ret.insert(0, tmp);
      free(tmp);
    }
  }
  return ret;
}

std::string to_string(int n) {
  char* str;
  std::string res;

  str = ft_itoa(n);
  if (str == NULL) {
    throw new std::bad_alloc();
  }
  res = str;
  free(str);
  return res;
}
