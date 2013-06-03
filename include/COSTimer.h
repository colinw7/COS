#ifndef COSTimer_H
#define COSTimer_H

#include <sys/types.h>

namespace COSTimer {
  uint sleep(uint secs);

  uint micro_sleep(uint msecs);
  uint milli_sleep(uint msecs);
  uint msleep(uint msecs);

  bool alarm(int secs, int *remain);
}

#endif
