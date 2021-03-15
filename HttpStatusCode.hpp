/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpStatusCode.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/03 20:33:54 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/15 19:23:21 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPSTATUSCODE_HPP
#define HTTPSTATUSCODE_HPP

enum HTTPStatusCode {
  HTTP_100 = 100, /* Continue */
  HTTP_101 = 101, /* Switching Protoclols */

  HTTP_200 = 200, /* OK */
  HTTP_201 = 201, /* Created */
  HTTP_202 = 202, /* Accepted */
  HTTP_203 = 203, /* Non-Authoritative Information */
  HTTP_204 = 204, /* No Content */
  HTTP_205 = 205, /* Reset Content */

  HTTP_300 = 300, /* Multiple choice */
  HTTP_301 = 301, /* Moved Parmanently */
  HTTP_302 = 302, /* Found */
  HTTP_303 = 303, /* See Other */
  HTTP_307 = 307, /* Temporary Redirect */

  HTTP_400 = 400, /* Bad Request */
  HTTP_404 = 404, /* Forbidden */
  HTTP_405 = 405, /* Method Not Allowed */
  HTTP_406 = 406, /* Not Acceptable */
  HTTP_408 = 408, /* Request Timeout */
  HTTP_409 = 409, /* Conflict */
  HTTP_410 = 410, /* Gone */
  HTTP_411 = 411, /* Length Required */
  HTTP_413 = 413, /* Pyaload Too Large */
  HTTP_415 = 415, /* URI Too Long */
  HTTP_417 = 417, /* Exception Failed */
  HTTP_426 = 426, /* Upgrade Required */

  HTTP_500 = 500, /* Internal Server Error */
  HTTP_501 = 501, /* Not Implemented */
  HTTP_502 = 502, /* Bad Gateway */
  HTTP_503 = 503, /* Service Unavailable */
  HTTP_504 = 504, /* Gateway Timeout */
  HTTP_505 = 505  /* HTTP Version Not Supported */
};

#endif /* HTTPSTATUSCODE_HPP */
