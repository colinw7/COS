#ifndef COSTime_H
#define COSTime_H

#include <CHRTime.h>

#include <sys/types.h>
#include <string>

namespace COSTime {
  double getFTime();

  std::string getTimeString(const std::string &format="%c");
  std::string getTimeString(time_t t, const std::string &format="%c");

  bool decodeTimeString(const std::string &time_string, const std::string &format, time_t &t);

  void getTime(int *year, int *month, int *day, int *hour, int *min, int *sec);
  void getTime(time_t t, int *year, int *month, int *day, int *hour, int *min, int *sec);

  time_t encodeTime(int year, int month, int day, int hour, int min, int sec);

  CHRTime getHRTime();
  CHRTime diffHRTime(const CHRTime &time1, const CHRTime &time2);

  void getHRTime(CHRTime &t);
  void getHRTime(long *secs, long *usecs);

  void diffHRTime(long secs1, long usecs1, long secs2, long usecs2, long *decs, long *dusecs);
  void diffHRTime(long secs1, long usecs1, long secs2, long usecs2, long *dusecs);
  void diffHRTime(long secs1, long usecs1, long secs2, long usecs2, double *decs);

  uint getElapsedCPU();

  double getUserCPU();
  double getSystemCPU();
}

#endif
