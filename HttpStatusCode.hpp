/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpStatusCode.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/03 20:33:54 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/19 02:17:30 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPSTATUSCODE_HPP
#define HTTPSTATUSCODE_HPP

enum HTTPStatusCode {
  HTTP_NOMATCH = -1, /* No Match to HTTP codes (used in internal function) */

  HTTP_100 = 100, /* Continue */
  HTTP_101 = 101, /* Switching Protoclols */
  HTTP_102 = 102, /* Processing (RFC2518) */
  HTTP_103 = 103, /* Early Hints  (RFC8297)*/

  HTTP_200 = 200, /* OK */
  HTTP_201 = 201, /* Created */
  HTTP_202 = 202, /* Accepted */
  HTTP_203 = 203, /* Non-Authoritative Information */
  HTTP_204 = 204, /* No Content */
  HTTP_205 = 205, /* Reset Content */
  HTTP_206 = 206, /* Partial Content */
  HTTP_207 = 207, /* Multi-Status (RFC4918) */
  HTTP_208 = 208, /* Already Reported (RFC5842) */
  HTTP_226 = 226, /* IM Used (RFC3229) */

  HTTP_300 = 300, /* Multiple choice */
  HTTP_301 = 301, /* Moved Parmanently */
  HTTP_302 = 302, /* Found */
  HTTP_303 = 303, /* See Other */
  HTTP_304 = 304, /* Not Modified */
  HTTP_305 = 305, /* Use Proxy */
  HTTP_306 = 306, /* Unused */
  HTTP_307 = 307, /* Temporary Redirect */
  HTTP_308 = 308, /* Permanent Redirect (RFC7538) */

  HTTP_400 = 400, /* Bad Request */
  HTTP_401 = 401, /* Unauthorized */
  HTTP_402 = 402, /* Payment Required */
  HTTP_403 = 403, /* Forbidden */
  HTTP_404 = 404, /* Not Found */
  HTTP_405 = 405, /* Method Not Allowed */
  HTTP_406 = 406, /* Not Acceptable */
  HTTP_407 = 407, /* Proxy Authentication Required */
  HTTP_408 = 408, /* Request Timeout */
  HTTP_409 = 409, /* Conflict */
  HTTP_410 = 410, /* Gone */
  HTTP_411 = 411, /* Length Required */
  HTTP_412 = 412, /* Precondition Failed */
  HTTP_413 = 413, /* Payload Too Large */
  HTTP_414 = 414, /* URI Too Long */
  HTTP_415 = 415, /* Unsupported Media Type */
  HTTP_416 = 416, /* Range Not Satisfiable */
  HTTP_417 = 417, /* Exception Failed */
  HTTP_418 = 418, /* I'm a teapot */
  HTTP_421 = 421, /* Misdirected Request (RFC7540) */
  HTTP_422 = 422, /* Unprocessable Entity (RFC4918) */
  HTTP_423 = 423, /* Locked (RFC4918) */
  HTTP_424 = 424, /* Failed Dependency (RFC4918) */
  HTTP_425 = 425, /* Too Early (RFC8470) */
  HTTP_426 = 426, /* Upgrade Required */
  HTTP_428 = 428, /* Precondition Required (RFC6585) */
  HTTP_429 = 429, /* Too Many Requests (RFC6585) */
  HTTP_431 = 431, /* Request Header Fields Too Large (RFC6585) */
  HTTP_451 = 451, /* Unabailable For Legal Reasons (RFC7225) */

  HTTP_500 = 500, /* Internal Server Error */
  HTTP_501 = 501, /* Not Implemented */
  HTTP_502 = 502, /* Bad Gateway */
  HTTP_503 = 503, /* Service Unavailable */
  HTTP_504 = 504, /* Gateway Timeout */
  HTTP_505 = 505, /* HTTP Version Not Supported */
  HTTP_506 = 506, /* Variant Also Negotiates (RFC2295) */
  HTTP_507 = 507, /* Insufficient Storage (RFC4918) */
  HTTP_508 = 508, /* HTTP Version Not Supported (RFC5842) */
  HTTP_510 = 510, /* Not Extended (RFC2774) */
  HTTP_511 = 511  /* Network Authentication Required (RFC6585) */
};

#endif /* HTTPSTATUSCODE_HPP */
