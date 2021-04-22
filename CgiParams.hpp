/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiParams.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/22 22:28:46 by dhasegaw          #+#    #+#             */
/*   Updated: 2021/04/22 22:30:49 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIPARAMS_HPP
#define CGIPARAMS_HPP

#include <string>

#include "Request.hpp"
#include "Session.hpp"

class CgiParams {
 private:
  const Session& session_;
  char** argv_;
  char** envp_;

  CgiParams();
  CgiParams(CgiParams const& other);
  CgiParams& operator=(CgiParams const& other);

 public:
  CgiParams(Session const& session);
  ~CgiParams();

  char** storeArgv(const std::string& filepath, const std::string& cgiuri,
                   const Request& request);
  char** storeMetaVariables(const std::string& cgiuri, const Request& request);

 private:
  char** vecToChar(std::vector<std::string>& meta_variables);
};

#endif /* CGIPARAMS_HPP */
