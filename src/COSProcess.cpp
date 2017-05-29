#include <COSProcess.h>
#include <COSSignal.h>

#include <std_os.h>

#include <cerrno>
#include <signal.h>

#ifdef OS_UNIX
# include <sys/wait.h>
#endif

pid_t
COSProcess::
getProcessId()
{
  return getpid();
}

pid_t
COSProcess::
getParentProcessId()
{
  return getppid();
}

pid_t
COSProcess::
getSessionId()
{
  return getSessionId(0);
}

pid_t
COSProcess::
getSessionId(pid_t pid)
{
  return getsid(pid);
}

pid_t
COSProcess::
getProcessGroupId()
{
  return getpgid(0);
}

bool
COSProcess::
setProcessGroupId(pid_t pid)
{
  return setProcessGroupId(pid, pid);
}

bool
COSProcess::
setProcessGroupId(pid_t pid, pid_t pgrp)
{
  int error = setpgid(pid, pgrp);

  if (error == -1) return false;

  return true;
}

bool
COSProcess::
killSelf(int signal)
{
  int error = raise(signal);

  if (error != 0)
    return false;
  else
    return true;
}

bool
COSProcess::
killProcess(pid_t pid, int signal)
{
  int error = kill(pid, signal);

  if (error != 0)
    return false;
  else
    return true;
}

bool
COSProcess::
killProcessGroup(pid_t pgrp, int signal)
{
  int error = killpg(pgrp, signal);

  if (error != 0)
    return false;
  else
    return true;
}

bool
COSProcess::
isProcess(pid_t pid)
{
  int error = kill(pid, 0);

  if (error != 0) {
    if (errno == ESRCH) // doesn't exist
      return false;

    // errno == EPERM means exist but don't have permission to kill
  }

  return true;
}

bool
COSProcess::
waitAnyProcess(pid_t *pid, int *status)
{
#ifdef OS_UNIX
  pid_t tpid = ::waitpid(-1, status, 0);

  *pid = tpid;

  if (tpid == -1) return false;

  return true;
#else
  std::cerr << "COSProcess::waitAnyProcess: Unimplemented" << std::endl;

  return false;
#endif
}

bool
COSProcess::
waitProcess(pid_t *pid, int options, int *status)
{
#ifdef OS_UNIX
  pid_t tpid = ::waitpid(*pid, status, options);

  if (tpid == -1) return false;

  if ((*pid == 0 || *pid == -1) && (tpid != 0 && tpid != -1))
    *pid = tpid;

  return true;
#else
  std::cerr << "COSProcess::waitProcess: Unimplemented" << std::endl;

  return false;
#endif
}

int
COSProcess::
system(const char *command)
{
  // Check if shell is available
  if (! command)
    return isShellAvailable();

  /* The parent process (the caller of system()) blocks SIGCHLD and ignores
     SIGINT and SIGQUIT while the child is executing. We must change the signal
     settings prior to forking, to avoid possible race conditions. This means
     that we must undo the effects of the following in the child after fork(). */

  // TODO: fail if SIGCHLD is set to SIG_IGN

  /* Block SIGCHLD */
  COSSignal::blockSignal(SIGCHLD); sigset_t origMask = COSSignal::getOldSigSet();

  /* Ignore SIGINT and SIGQUIT */
  COSSignal::ignoreSignal(SIGINT ); struct sigaction saOrigInt  = COSSignal::getOldSigAction();
  COSSignal::ignoreSignal(SIGQUIT); struct sigaction saOrigQuit = COSSignal::getOldSigAction();

  int status = 0;

  pid_t childPid;

  switch (childPid = fork()) {
    /* fork() failed */
    case -1:
      status = -1;
      break; /* Carry on to reset signal attributes */

    /* Child: exec command */
    case 0: {
      /* We ignore possible error returns because the only specified error
         is for a failed exec(), and because errors in these calls can't
         affect the caller of system() (which is a separate process) */

      if (saOrigInt.sa_handler != SIG_IGN)
        COSSignal::defaultSignal(SIGINT);

      if (saOrigQuit.sa_handler != SIG_IGN)
        COSSignal::defaultSignal(SIGQUIT);

      (void) COSSignal::setSigProcMask(origMask);

      // TODO: if /bin/sh is not standard SHELL then use default PATH to find shell
      //
      //  char path[PATH_MAX];
      //  confstr(_CS_PATH, path, PATH_MAX);
      //  setenv("PATH", path, /*overwrite*/1);
      //  execlp("sh", "sh", "-c", command, (char *) 0);

      execl("/bin/sh", "sh", "-c", command, (char *) 0);

      _exit(127); /* We could not exec the shell */
    }

    /* Parent: wait for our child to terminate */
    default: {
      /* We must use waitpid() for this task; using wait() could inadvertently
         collect the status of one of the caller's other children */

      while (! COSProcess::waitProcess(&childPid, 0, &status)) {
        if (errno != EINTR) { /* Error other than EINTR */
          status = -1;
          break; /* So exit loop */
        }
      }

      break;
    }
  }

  // Parent and failed ...

  /* Unblock SIGCHLD, restore dispositions of SIGINT and SIGQUIT */

  int savedErrno = errno; /* The following may change 'errno' */

  (void) COSSignal::setSigProcMask(origMask);

  (void) COSSignal::setSigAction(SIGINT , saOrigInt );
  (void) COSSignal::setSigAction(SIGQUIT, saOrigQuit);

  errno = savedErrno;

  return status;
}

//------

int
COSProcess::
fork()
{
#ifdef OS_UNIX
  int error = ::fork();

  return error;
#else
  std::cerr << "COSProcess::fork: Unimplemented" << std::endl;

  return 0;
#endif
}

bool
COSProcess::
pipe(int *ifd, int *ofd)
{
#ifdef OS_UNIX
  int fd[2];

  fd[0] = *ifd;
  fd[1] = *ofd;

  int error = ::pipe(fd);

  if (error != 0)
    return false;

  *ifd = fd[0];
  *ofd = fd[1];

  return true;
#else
  std::cerr << "COSProcess::pipe: Unimplemented" << std::endl;

  return false;
#endif
}

//------

static COSProcess::CommandState command_state_;

bool
COSProcess::
executeCommand(const std::string &command, CommandState *state)
{
  std::string output;

  return executeCommand(command, output, state);
}

bool
COSProcess::
executeCommand(const std::string &command, std::string &output, CommandState *state)
{
  output = "";

  FILE *fp = openProcess(command, "r");

  if (! fp)
    return false;

  int c;

  while ((c = fgetc(fp)) != EOF)
    output += c;

  bool rc = closeProcess(fp);

  if (state)
    *state = command_state_;

  return rc;
}

FILE *
COSProcess::
openProcess(const std::string &command, const std::string &mode)
{
#ifdef OS_UNIX
  FILE *fp = popen(command.c_str(), mode.c_str());

  return fp;
#else
  std::cerr << "COSProcess::openProcess: Unimplemented" << std::endl;

  return nullptr;
#endif
}

bool
COSProcess::
closeProcess(FILE *fp)
{
#ifdef OS_UNIX
  int rc = pclose(fp);

  command_state_.status_ = WEXITSTATUS(rc);

  if      (WIFSIGNALED(rc)) {
    command_state_.signaled_ = true;
    command_state_.signum_   = WTERMSIG(rc);
  }
  else if (WIFSTOPPED(rc)) {
    command_state_.stopped_ = true;
    command_state_.signum_  = WSTOPSIG(rc);
  }
  else {
    command_state_.stopped_ = true;
    command_state_.signum_  = 0;
  }

  if (rc < 0)
    return false;

  return true;
#else
  std::cerr << "COSProcess::closeProcess: Unimplemented" << std::endl;

  return false;
#endif
}

bool
COSProcess::
isShellAvailable()
{
  return (COSProcess::system(":") == 0);
}
