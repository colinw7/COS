#ifndef COSEnv_H
#define COSEnv_H

#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <unistd.h>

namespace COSEnv {
  inline bool checkenv(const std::string &name) {
    return (getenv((char *) name.c_str()) != NULL);
  }

  inline std::string getenv(const std::string &name) {
    char *env = ::getenv((char *) name.c_str());

    return (env ? std::string(env) : "");
  }

  inline void setenv(const std::string &name, const std::string &value) {
    std::string env = name + "=" + value;

    char *envp = strdup((char *) env.c_str());

    /*int error = */ ::putenv(envp);
  }

  inline void setenv(const std::string &name, int value) {
    char str[128];

    ::sprintf(str, "%d", value);

    setenv(name, str);
  }

  inline void unsetenv(const std::string &name) {
    int name_len = name.size();

    int i = 0;

    while (environ[i] != NULL) {
      int environ_len = strlen(environ[i]);

      if (environ_len > name_len &&
          strncmp(environ[i], name.c_str(), name_len) == 0 &&
          environ[i][name_len] == '=')
        break;

      i++;
    }

    if (environ[i] == NULL)
      return;

    int i1 = i;

    while (environ[i1] != NULL)
      i1++;

    for (int j = i; j < i1; j++)
      environ[j] = environ[j + 1];
  }

  inline void getNameValues(std::vector<std::string> &names, std::vector<std::string> &values) {
    names .clear();
    values.clear();

    for (int i = 0; environ[i] != NULL; i++) {
      std::string env_name = std::string(environ[i]);

      std::string::size_type pos = env_name.find('=');

      std::string env_value;

      if (pos != std::string::npos) {
        env_value = env_name.substr(pos + 1);
        env_name  = env_name.substr(0, pos);
      }

      names .push_back(env_name );
      values.push_back(env_value);
    }
  }

  typedef std::pair<std::string,std::string> NameValue;

  class NameValueCmp {
   public:
    int operator()(const NameValue &a, const NameValue &b) {
      return (a.first < b.first);
    }
  };

  inline void getSortedNameValues(std::vector<std::string> &names,
                                  std::vector<std::string> &values) {
    std::vector<NameValue> name_values;

    for (int i = 0; environ[i] != NULL; i++) {
      std::string env_name = std::string(environ[i]);

      std::string::size_type pos = env_name.find('=');

      std::string env_value;

      if (pos != std::string::npos) {
        env_value = env_name.substr(pos + 1);
        env_name  = env_name.substr(0, pos);
      }

      name_values.push_back(NameValue(env_name,env_value));
    }

    std::sort(name_values.begin(), name_values.end(), NameValueCmp());

    names .clear();
    values.clear();

    for (unsigned int i = 0; i < name_values.size(); i++) {
      names .push_back(name_values[i].first );
      values.push_back(name_values[i].second);
    }
  }
}

#endif
