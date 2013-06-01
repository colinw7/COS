#include <COSSignal.h>
#include <COSProcess.h>

#include <vector>
#include <cstdarg>
#include <cassert>
#include <cerrno>
#include <cstring>
#include <iostream>

static sigset_t              cos_old_sigset;
static std::vector<sigset_t> cos_signal_stack;
static struct sigaction      cos_old_sigaction;

void
COSSignal::
dummyHandler(int)
{
}

bool
COSSignal::
blockAllSignals()
{
  sigset_t set;

  sigemptyset(&set);

  if (::sigprocmask(SIG_SETMASK, &set, &cos_old_sigset) == -1)
    return false;

  return true;
}

bool
COSSignal::
blockSignal(int sig)
{
  return blockSignals(sig, 0);
}

bool
COSSignal::
blockSignals(int sig, ...)
{
  va_list vargs;

  va_start(vargs, sig);

  sigset_t set;

  sigemptyset(&set);

  while (sig) {
    sigaddset(&set, sig);

    sig = va_arg(vargs, int);
  }

  va_end(vargs);

  if (::sigprocmask(SIG_BLOCK, &set, &cos_old_sigset) == -1)
    return false;

  return true;
}

bool
COSSignal::
unblockAllSignals()
{
  sigset_t set;

  sigfillset(&set);

  if (::sigprocmask(SIG_SETMASK, &set, &cos_old_sigset) == -1)
    return false;

  return true;
}

bool
COSSignal::
unblockSignal(int sig)
{
  return unblockSignals(sig, 0);
}

bool
COSSignal::
unblockSignals(int sig, ...)
{
  va_list vargs;

  va_start(vargs, sig);

  sigset_t set;

  sigemptyset(&set);

  while (sig) {
    sigaddset(&set, sig);

    sig = va_arg(vargs, int);
  }

  va_end(vargs);

  if (::sigprocmask(SIG_UNBLOCK, &set, &cos_old_sigset) == -1)
    return false;

  return true;
}

sigset_t
COSSignal::
getOldSigSet()
{
  return cos_old_sigset;
}

bool
COSSignal::
startBlockSignal(int sig)
{
  return startBlockSignals(sig, 0);
}

bool
COSSignal::
startBlockSignals(int sig, ...)
{
  va_list vargs;

  va_start(vargs, sig);

  sigset_t set;

  sigemptyset(&set);

  while (sig) {
    sigaddset(&set, sig);

    sig = va_arg(vargs, int);
  }

  va_end(vargs);

  sigset_t prev_set;

  if (::sigprocmask(SIG_BLOCK, &set, &prev_set) == -1)
    return false;

  cos_signal_stack.push_back(prev_set);

  return true;
}

bool
endBlockSignals()
{
  assert(! cos_signal_stack.empty());

  sigset_t set = cos_signal_stack.back();

  cos_signal_stack.pop_back();

  if (::sigprocmask(SIG_SETMASK, &set, &cos_old_sigset) == -1)
    return false;

  return true;
}

bool
COSSignal::
setOldSigProcMask()
{
  return setSigProcMask(cos_old_sigset);
}

bool
COSSignal::
setSigProcMask(const sigset_t &set)
{
  if (::sigprocmask(SIG_SETMASK, &set, &cos_old_sigset) == -1)
    return false;

  return true;
}

bool
COSSignal::
unblockSignalAndWait(int sig)
{
  return unblockSignalsAndWait(sig, 0);
}

bool
COSSignal::
unblockSignalsAndWait(int sig, ...)
{
  va_list vargs;

  va_start(vargs, sig);

  sigset_t set;

  sigfillset(&set);

  while (sig) {
    sigdelset(&set, sig);

    sig = va_arg(vargs, int);
  }

  va_end(vargs);

  int error = sigsuspend(&set);

  if (error == -1 && errno != EINTR)
    return false;

  return true;
}

COSSignal::SignalHandler
COSSignal::
getSignalHandler(int sig)
{
  struct sigaction act;

  memset(&act, 0, sizeof(act));

  int rc = ::sigaction(sig, NULL, &act);

  if (rc < 0)
    return NULL;

  return act.sa_handler;
}

bool
COSSignal::
addSignalHandler(int sig, SignalHandler proc, uint opts)
{
  struct sigaction act;

  memset(&act, 0, sizeof(act));

  sigemptyset(&act.sa_mask);

  if (proc != NULL)
    act.sa_handler = proc;
  else
    act.sa_handler = dummyHandler;

  act.sa_flags = 0;

  if (opts & CSIGNAL_OPT_RESTART)
    act.sa_flags |= SA_RESTART;

  if (opts & CSIGNAL_OPT_INFO)
    act.sa_flags |= SA_SIGINFO;

#ifdef SA_NOCLDSTOP
  if (opts & CSIGNAL_OPT_NO_CHILD_STOP)
    act.sa_flags |= SA_NOCLDSTOP;
#endif

  int rc = ::sigaction(sig, &act, &cos_old_sigaction);

  if (rc < 0)
    return false;

  return true;
}

bool
COSSignal::
ignoreSignal(int sig)
{
  struct sigaction act;

  memset(&act, 0, sizeof(act));

  act.sa_handler = SIG_IGN;
  act.sa_flags   = 0;

  sigemptyset(&act.sa_mask);

  int rc = ::sigaction(sig, &act, &cos_old_sigaction);

  if (rc < 0)
    return false;

  return true;
}

bool
COSSignal::
defaultSignal(int sig)
{
  struct sigaction act;

  memset(&act, 0, sizeof(act));

  act.sa_handler = SIG_DFL;
  act.sa_flags   = 0;

  sigemptyset(&act.sa_mask);

  int rc = ::sigaction(sig, &act, &cos_old_sigaction);

  if (rc < 0)
    return false;

  return true;
}

bool
COSSignal::
isSignalIgnored(int sig)
{
  struct sigaction act;

  if (sigaction(sig, NULL, &act) == -1)
    return false;

  return (act.sa_handler == SIG_IGN);
}

bool
COSSignal::
isSignalDefault(int sig)
{
  struct sigaction act;

  if (sigaction(sig, NULL, &act) == -1)
    return false;

  return (act.sa_handler == SIG_DFL);
}

bool
COSSignal::
setSigAction(int sig, const struct sigaction &act)
{
  int rc = ::sigaction(sig, &act, &cos_old_sigaction);

  if (rc < 0)
    return false;

  return true;
}

bool
COSSignal::
waitAnySignal()
{
  int error = pause();

  if (error == -1 && errno != EINTR)
    return false;

  return true;
}

bool
COSSignal::
waitSignal(int sig)
{
  return waitSignals(sig, 0);
}

bool
COSSignal::
waitSignals(int sig, ...)
{
#ifndef OS_OSX
  va_list vargs;

  va_start(vargs, sig);

  sigset_t set;

  sigemptyset(&set);

  while (sig) {
    sigaddset(&set, sig);

    sig = va_arg(vargs, int);
  }

  va_end(vargs);

  siginfo_t info;

  int rc = sigwaitinfo(&set, &info);

  if (rc < 0)
    return false;

  return true;
#else
  assert(sig);

  return false;
#endif
}

bool
COSSignal::
timedWaitSignal(time_t secs, long nsecs, int sig)
{
  return timedWaitSignals(secs, nsecs, sig, 0);
}

bool
COSSignal::
timedWaitSignals(time_t secs, long nsecs, int sig, ...)
{
#ifndef OS_OSX
  va_list vargs;

  va_start(vargs, sig);

  sigset_t set;

  sigemptyset(&set);

  while (sig) {
    sigaddset(&set, sig);

    sig = va_arg(vargs, int);
  }

  va_end(vargs);

  siginfo_t info;

  struct timespec ts;

  ts.tv_sec  = secs;
  ts.tv_nsec = nsecs;

  int rc = sigtimedwait(&set, &info, &ts);

  if (rc < 0)
    return false;

  return true;
#else
  assert(secs && nsecs && sig);

  return false;
#endif
}

struct sigaction
COSSignal::
getOldSigAction()
{
  return cos_old_sigaction;
}

bool
COSSignal::
sendSignal(int sig)
{
  return sendSignal(COSProcess::getProcessId(), sig);
}

bool
COSSignal::
sendSignal(pid_t pid, int sig)
{
  int error = COSProcess::killProcess(pid, sig);

  if (error == -1)
    return false;

  return true;
}

bool
COSSignal::
sendSignalToProcessGroup(int sig)
{
  int error = COSProcess::killProcess(0, sig);

  if (error == -1)
    return false;

  return true;
}

bool
COSSignal::
sendSignalToProcessGroup(pid_t pgrp, int sig)
{
  int error = COSProcess::killProcessGroup(pgrp, sig);

  if (error == -1)
    return false;

  return true;
}

bool
COSSignal::
sendSignalToAll(int sig)
{
  int error = COSProcess::killProcess(-1, sig);

  if (error == -1)
    return false;

  return true;
}

bool
COSSignal::
sendSignalData(int sig, int val)
{
  return sendSignalData(COSProcess::getProcessId(), sig, val);
}

bool
COSSignal::
sendSignalData(pid_t pid, int sig, int value)
{
#ifdef OS_LINUX
  assert(sig >= SIGRTMIN && sig <= SIGRTMAX);

  union sigval val;

  val.sival_int = value;

  int error = sigqueue(pid, sig, val);

  if (error < 0)
    return false;
#else
  std::cerr << "sendSignalData(" << pid << "," << sig << "," << value << ")" << std::endl;
#endif

  return true;
}

void
COSSignal::
printSigset(FILE *of, const char *prefix, const sigset_t *sigset)
{
  int cnt = 0;

  for (int sig = 1; sig < NSIG; sig++) {
    if (sigismember(sigset, sig)) {
      ++cnt;

      fprintf(of, "%s%d (%s)\n", prefix, sig, strsignal(sig).c_str());
    }
  }

  if (cnt == 0)
    fprintf(of, "%s<empty signal set>\n", prefix);
}

int
COSSignal::
printSigMask(FILE *of, const char *msg)
{
  if (msg != NULL)
    fprintf(of, "%s", msg);

  sigset_t currMask;

  if (::sigprocmask(SIG_BLOCK, NULL, &currMask) == -1)
    return -1;

  printSigset(of, "\t\t", &currMask);

  return 0;
}

int
COSSignal::
printPendingSigs(FILE *of, const char *msg)
{
  if (msg != NULL)
    fprintf(of, "%s", msg);

  sigset_t pendingSigs;

  if (sigpending(&pendingSigs) == -1)
    return -1;

  printSigset(of, "\t\t", &pendingSigs);

  return 0;
}

const char *
COSSignal::
getSignalName(int sig)
{
  switch (sig) {
    case SIGTERM: return "Termination signal";
    case SIGBUS : return "Access to undefined portion of a memory object";
    case SIGFPE : return "Erroneous arithmetic operation";
    case SIGILL : return "Illegal instruction";
    case SIGSEGV: return "Invalid memory reference";
    case SIGSYS : return "Bad system call";
    case SIGXCPU: return "CPU-time limit exceeded";
    case SIGXFSZ: return "File-size limit exceeded";
    default     : return "???";
  }
}
