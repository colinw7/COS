#ifndef CSignalOpt_H
#define CSignalOpt_H

enum CSignalOpt {
  CSIGNAL_OPT_NONE         =0,
  CSIGNAL_OPT_RESTART      =(1<<0),
  CSIGNAL_OPT_INFO         =(1<<1),
  CSIGNAL_OPT_NO_CHILD_STOP=(1<<2)
};

#endif
