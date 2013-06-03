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

  time_t encodeTime(int year, int month, int day, int hour, int min, int sec);

  CHRTime getHRTime();
  CHRTime diffHRTime(const CHRTime &time1, const CHRTime &time2);

  void getHRTime(int *secs, int *usecs);
  void diffHRTime(int secs1, int usecs1, int secs2, int usecs2, int *decs, int *dusecs);
  void diffHRTime(int secs1, int usecs1, int secs2, int usecs2, int *dusecs);
  void diffHRTime(int secs1, int usecs1, int secs2, int usecs2, double *decs);

  uint getElapsedCPU();

  double getUserCPU();
  double getSystemCPU();
}

#endif
