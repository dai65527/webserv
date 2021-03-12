#ifndef SERVERLOCATIONCONFIG_HPP
#define SERVERLOCATIONCONFIG_HPP

#include <string>

/*
** ServerLocationConfig
**
** Base class of ServerConfig and LocationConfig
** Includes directives which are only avairable in server and location context
*/

class ServerLocationConfig {
 protected:
  std::string upload_pass_;
  std::string upload_store_;
 public:
  // coplien
  ServerLocationConfig();
  ServerLocationConfig(const ServerLocationConfig& ref);
  ServerLocationConfig& operator=(const ServerLocationConfig& ref);
  virtual ~ServerLocationConfig();

  // getters
  const std::string& getUploadPass() const;
  const std::string& getUploadStore() const;

  /*
  ** persers
  **
  ** Take content settings as string and then parse it and store the results.
  ** Return values:
  **  0: successfully parsed and stored the result
  **  1: syntax error
  **  2: wrong value was set
  **  3: duplicate value (only for directive cannot be duplicated)
  */

  int parseUploadPass(const std::string& settings);
  int parseUploadStore(const std::string& settings);
};

#endif /* SERVERLOCATIONCONFIG_HPP */
