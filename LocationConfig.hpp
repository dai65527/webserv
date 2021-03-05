/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/01 23:59:30 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/05 01:02:05 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONCONFIG_HPP
#define LOCATIONCONFIG_HPP

#include <list>
#include <string>

class LocationConfig {
 private:
  std::string location_;
  std::string root_;
  uint32_t allowed_method_;
  std::list<std::string> index_;
  bool auto_index_;
  std::string upload_pass_;
  std::string upload_store_;
  std::list<std::string> cgi_extension_;

 public:
  LocationConfig();
  virtual ~LocationConfig();
  LocationConfig(const LocationConfig& other);
  LocationConfig& operator=(const LocationConfig& other);

  const std::string& getLocation() const;
  const std::string& getRoot() const;
  unsigned int getAllowedMethod() const;
  const std::list<std::string>& getIndex() const;
  bool getAutoIndex() const;
  const std::string& getUploadPass() const;
  const std::string& getUploadStore() const;
  const std::list<std::string>& getCgiExtension() const;

  void setLocation(const std::string& location);
  void setRoot(const std::string& root);
  void setAllowedMethod(const std::string& allowed_method);
  void addIndex(const std::string& index);
  void setAutoIndex(bool);
  void setUploadPass(const std::string& upload_pass);
  void setUploadStore(const std::string& upload_store);
  void setCgiExtension(const std::string& cgi_extension);
};

#endif /* LOCATIONCONFIG_HPP */
