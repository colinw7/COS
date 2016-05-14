#include <COSStrSignal.h>

struct SigName {
  int         num;
  const char *name;
};

static SigName
signames[] = {
  { SIGHUP  , "Hangup"                   },
  { SIGINT  , "Interrupt"                },
#ifdef SIGQUIT
  { SIGQUIT , "Quit"                     },
#endif
  { SIGILL  , "Illegal Instruction"      },
  { SIGTRAP , "Trace/breakpoint trap"    },
  { SIGIOT  , "Abort"                    },
  { SIGFPE  , "Floating point exception" },
  { SIGKILL , "Kill"                     },
  { SIGUSR1 , "User signal 1"            },
  { SIGUSR2 , "User signal 2"            },
  { SIGPIPE , "Broken pipe"              },
#ifdef SIGALRM
  { SIGALRM , "Alarm"                    },
#endif
  { SIGTERM , "Terminate"                },
#ifdef SIGCHLD
  { SIGCHLD , "Child Died"               },
#endif
  { SIGCONT , "Continue"                 },
  { SIGSTOP , "Stop"                     },
  { SIGTSTP , "Stop on TTY"              },
  { SIGTTIN , "No TTY for Input"         },
  { SIGTTOU , "No TTY for Output"        },
  { SIGWINCH, "Window Resized"           },
  { SIGSEGV , "segmentation Fault"       },
  {       0 , NULL                       }
};

std::string
COSSignal::
strsignal(int sig)
{
  return signal_to_string(sig);
}

std::string
COSSignal::
signal_to_string(int sig)
{
#ifndef __linux__
  for (uint i = 0; signames[i].name != NULL; ++i)
    if (signame[i].num == sig)
      return signames[i].name;

  return "???";
#else
  return ::strsignal(sig);
#endif
}

int
COSSignal::
string_to_signal(const std::string &name)
{
  uint len = name.size();

  std::string name1 = name;

  for (uint i = 0; i < len; ++i)
    name1[i] = tolower(name[i]);

  for (uint i = 0; signames[i].name != NULL; ++i)
    if (signames[i].name == name1)
      return signames[i].num;

  return -1;
}
