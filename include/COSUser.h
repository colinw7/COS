#ifndef COSUser_H
#define COSUser_H

#include <sys/types.h>
#include <string>
#include <vector>

// User/Group OS functions

namespace COSUser {
  uint getUserId();
  bool getUserId(uint *uid);
  uint getUserId(const std::string &name);
  bool getUserId(const std::string &name, uint *uid);

  uint getEffectiveUserId();
  bool getEffectiveUserId(uint *uid);

  bool isUserName(const std::string &name);

  std::string getUserName();
  bool        getUserName(std::string &name);

  std::string getEffectiveUserName();
  bool        getEffectiveUserName(std::string &name);

  std::string getUserName(uint uid);
  bool        getUserName(uint uid, std::string &name);

  std::string getUserDesc();
  bool        getUserDesc(std::string &desc);

  std::string getEffectiveUserDesc();
  bool        getEffectiveUserDesc(std::string &desc);

  std::string getUserDesc(const std::string &name);
  bool        getUserDesc(const std::string &name, std::string &desc);
  std::string getUserDesc(uint uid);
  bool        getUserDesc(uint uid, std::string &desc);

  std::string getUserHome();
  bool        getUserHome(std::string &home);

  std::string getEffectiveUserHome();
  bool        getEffectiveUserHome(std::string &home);

  std::string getUserHome(const std::string &name);
  bool        getUserHome(const std::string &name, std::string &home);
  std::string getUserHome(uint uid);
  bool        getUserHome(uint uid, std::string &home);

  uint getUserGroupId();
  uint getEffectiveUserGroupId();

  uint getUserGroupId(const std::string &name);
  uint getUserGroupId(uint uid);

  std::string getUserShell();
  std::string getEffectiveUserShell();

  std::string getUserShell(const std::string &name);
  std::string getUserShell(uint uid);

  uint getGroupId();
  bool getGroupId(uint *gid);
  uint getGroupId(const std::string &name);
  bool getGroupId(const std::string &name, uint *gid);

  bool isGroupName(const std::string &name);

  std::string getGroupName();
  std::string getGroupName(uint gid);

  uint getUMask();
  bool setUMask(uint mask);

  std::string getHostName();

  bool getUsers(std::vector<std::string> &names);

  const char *getShellName();

  // Error Message
  const std::string &getErrorMsg();
  void setErrorMsg(const std::string &str);
}

#endif
