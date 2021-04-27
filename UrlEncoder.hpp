/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UrlEncoder.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 11:12:45 by dnakano           #+#    #+#             */
/*   Updated: 2021/04/23 11:54:43 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef URLENCODER_HPP
#define URLENCODER_HPP

#include <string>

class UrlEncoder {
 private:
  UrlEncoder();
  ~UrlEncoder();
  UrlEncoder(const UrlEncoder& ref);
  UrlEncoder& operator=(const UrlEncoder& rhs);

 public:
  static std::string encode(const int c);
  static std::string encode(const std::string& str);
  static std::string decode(const std::string& str);
};

#endif /* URLENCODER_HPP */
