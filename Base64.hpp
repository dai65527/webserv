/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Base64.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/29 14:38:27 by dnakano           #+#    #+#             */
/*   Updated: 2021/04/29 17:02:35 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASE64_HPP
#define BASE64_HPP

#include <map>
#include <string>
#include <vector>

class Base64 {
 private:
  static std::map<char, char> encodemap;
  static std::map<char, char> decodemap;
  static void initEncodemap();
  static void initDecodemap();

 public:
  static std::string encode(const unsigned char *data, size_t length);
  static std::string encode(const std::string& data);
  static std::string decode(const std::string& data);
};

#endif /* BASE64_HPP */
