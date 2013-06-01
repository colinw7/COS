#ifndef COSSignal_H
#define COSSignal_H

#include <CSignalOpt.h>

#include <cstdio>
#include <sys/signal.h>
#include <sys/types.h>
#include <string>

namespace COSSignal {
  typedef void (*SignalHandler)(int sig);

  bool blockAllSignals();

  bool blockSignal(int sig);
  bool blockSignals(int sig, ...);

  bool startBlockSignal(int sig);
  bool startBlockSignals(int sig, ...);
  bool endBlockSignals();

  bool unblockAllSignals();

  bool unblockSignal(int sig);
  bool unblockSignals(int sig, ...);

  bool unblockSignalAndWait(int sig);
  bool unblockSignalsAndWait(int sig, ...);

  bool setOldSigProcMask();
  bool setSigProcMask(const sigset_t &set);

  sigset_t getOldSigSet();

  SignalHandler getSignalHandler(int sig);

  bool addSignalHandler(int sig, SignalHandler proc, uint opts=CSIGNAL_OPT_NONE);

  bool ignoreSignal (int sig);
  bool defaultSignal(int sig);

  bool isSignalIgnored(int sig);
  bool isSignalDefault(int sig);

  bool setSigAction(int sig, const struct sigaction &act);

  bool waitAnySignal();

  bool waitSignal (int sig);
  bool waitSignals(int sig, ...);

  bool timedWaitSignal(time_t secs, long nsecs, int sig);
  bool timedWaitSignals(time_t secs, long nsecs, int sig, ...);

  struct sigaction getOldSigAction();

  bool sendSignal(int sig);
  bool sendSignal(pid_t pid, int sig);

  bool sendSignalToProcessGroup(int sig);
  bool sendSignalToProcessGroup(pid_t pgrp, int sig);

  bool sendSignalToAll(int sig);

  bool sendSignalData(int sig, int value);
  bool sendSignalData(pid_t pid, int sig, int value);

  void printSigset(FILE *of, const char *prefix, const sigset_t *sigset);
  int  printSigMask(FILE *of, const char *msg);
  int  printPendingSigs(FILE *of, const char *msg);

  const char *getSignalName(int sig);

  void dummyHandler(int sig);

  std::string strsignal(int sig);

  std::string signal_to_string(int sig);
  int         string_to_signal(const std::string &name);
}

#endif
