#ifndef COSProcess_H
#define COSProcess_H

#include <sys/types.h>
#include <unistd.h>
#include <string>

namespace COSProcess {
  struct CommandState {
    int  status_;
    bool signaled_;
    bool stopped_;
    int  signum_;

    int  getStatus () { return status_  ; }
    bool isSignaled() { return signaled_; }
    bool isStopped () { return stopped_ ; }
    int  getSignum () { return signum_  ; }
  };

  pid_t getProcessId();

  pid_t getParentProcessId();

  pid_t getSessionId();
  pid_t getSessionId(pid_t pid);

  pid_t getProcessGroupId();
  bool  setProcessGroupId(pid_t pid);
  bool  setProcessGroupId(pid_t pid, pid_t pgrp);

  bool killSelf(int signal);
  bool killProcess(pid_t pid, int signal);
  bool killProcessGroup(pid_t pgrp, int signal);
  bool isProcess(pid_t pid);

  bool waitAnyProcess(int *pid, int *status);
  bool waitProcess(int *pid, int options, int *status);

  int system(const char *command);

  int fork();

  bool pipe(int *ifd, int *ofd);

  bool executeCommand(const std::string &command, CommandState *state=nullptr);
  bool executeCommand(const std::string &command, std::string &output,
                      CommandState *state=nullptr);

  FILE *openProcess(const std::string &command, const std::string &mode);
  bool  closeProcess(FILE *fp);

  bool isShellAvailable();
}

#endif
