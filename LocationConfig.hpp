/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/01 23:59:30 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/03/03 10:26:25 by dhasegaw         ###   ########.fr       */
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
  uint allowed_method_;
  std::list<std::string> index_;
  bool auto_index_;
  std::string upload_pass_;
  std::string upload_store_;
  std::list<std::string> cgi_extension_;

 public:
  LocationConfig();
  virtual ~LocationConfig();
  LocationConfig(LocationConfig const &);
  LocationConfig &operator=(LocationConfig const &);

  std::string getLocation() const;
  std::string getRoot() const;
  unsigned int getAllowedMethod() const;
  std::list<std::string> getIndex() const;
  bool getAutoIndex() const;
  std::string getUploadPass() const;
  std::string getUploadStore() const;
  std::list<std::string> getCgiExtension() const;

  void setLocation(std::string);
  void setRoot(std::string);
  void setAllowedMethod(std::string);
  void setIndex(std::list<std::string>);
  void setAutoIndex(bool);
  void setUploadPass(std::string);
  void setUploadStore(std::string);
  void setCgiExtension(std::list<std::string>);
};

#endif /* LOCATIONCONFIG_HPP */
