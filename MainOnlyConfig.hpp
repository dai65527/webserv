#ifndef MAINONLYCONFIG_HPP
#define MAINONLYCONFIG_HPP

#include <string>

/*
** ServerLocationConfig
**
** Base class of MainConfig
** Includes directives which are only avairable in main context
*/

class MainOnlyConfig {
 protected:
  int max_sessions_;  // max_sessions directive
  int retry_after_;   // retry_after directive (times in sec)

 public:
  // coplien
  MainOnlyConfig();
  MainOnlyConfig(const MainOnlyConfig& ref);
  MainOnlyConfig& operator=(const MainOnlyConfig& ref);
  virtual ~MainOnlyConfig();

  // getters
  int getMaxSessions() const;
  int getRetryAfter() const;

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

  int parseMaxSessions(const std::string& settings);
  int parseRetryAfter(const std::string& settings);
};

#endif /* MAINONLYCONFIG_HPP */
