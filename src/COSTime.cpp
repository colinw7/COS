#include <COSTime.h>

#include <cstring>
#include <sys/time.h>
#include <sys/timeb.h>
#include <unistd.h>

#define OS_UNIX 1

#define HAS_SYS_TIMES 1

#ifdef HAS_SYS_TIMES
#include <sys/times.h>
#endif

double
COSTime::
getFTime()
{
#if OS_UNIX
  struct timeb tp;

  ftime(&tp);

  return tp.time + tp.millitm/1000.0;
#else
  std::cerr << "COSTime::getFTime: Unimplemented" << std::endl;

  return 0.0;
#endif
}

std::string
COSTime::
getTimeString(const std::string &format)
{
  time_t t = time(0);

  return getTimeString(t, format);
}

std::string
COSTime::
getTimeString(time_t t, const std::string &format)
{
  static char time_string[256];

  struct tm *tm = localtime(&t);

  if (! tm)
    tm = gmtime(&t);

  strftime(time_string, 256, format.c_str(), tm);

  return time_string;
}

bool
COSTime::
decodeTimeString(const std::string &time_string, const std::string &format, time_t &t)
{
  struct tm tm;

  memset(&tm, 0, sizeof(tm));

  char *p = strptime(time_string.c_str(), format.c_str(), &tm);

  if (! p)
    return false;

  tm.tm_wday  = 0;
  tm.tm_yday  = 0;
  tm.tm_isdst = -1; // auto DST

  t = mktime(&tm);

  return true;
}

void
COSTime::
getTime(int *year, int *month, int *day, int *hour, int *min, int *sec)
{
  time_t t = time(0);

  getTime(t, year, month, day, hour, min, sec);
}

void
COSTime::
getTime(time_t t, int *year, int *month, int *day, int *hour, int *min, int *sec)
{
  struct tm *tm = localtime(&t);

  if (! tm)
    tm = gmtime(&t);

  if (year ) *year  = tm->tm_year;
  if (month) *month = tm->tm_mon;
  if (day  ) *day   = tm->tm_mday;
  if (hour ) *hour  = tm->tm_hour;
  if (min  ) *min   = tm->tm_min;
  if (sec  ) *sec   = tm->tm_sec;
}

time_t
COSTime::
encodeTime(int year, int month, int day, int hour, int min, int sec)
{
  struct tm tm;

  tm.tm_year  = year;
  tm.tm_mon   = month;
  tm.tm_mday  = day;
  tm.tm_hour  = hour;
  tm.tm_min   = min;
  tm.tm_sec   = sec;
  tm.tm_wday  = 0;
  tm.tm_yday  = 0;
  tm.tm_isdst = -1; // auto DST

  return mktime(&tm);
}

//------

CHRTime
COSTime::
getHRTime()
{
  CHRTime hrtime;

  getHRTime(hrtime);

  return hrtime;
}

void
COSTime::
getHRTime(long *secs, long *usecs)
{
  CHRTime t = getHRTime();

  *secs  = t.secs;
  *usecs = t.usecs;
}

CHRTime
COSTime::
diffHRTime(const CHRTime &hrtime1, const CHRTime &hrtime2)
{
  CHRTime hrtime;

  diffHRTime(hrtime1.secs, hrtime1.usecs, hrtime2.secs, hrtime2.usecs, &hrtime.secs, &hrtime.usecs);

  return hrtime;
}

void
COSTime::
getHRTime(CHRTime &t)
{
  struct timeval timeval;

  gettimeofday(&timeval, 0);

  t.secs  = timeval.tv_sec;
  t.usecs = timeval.tv_usec;
}

void
COSTime::
diffHRTime(long secs1, long usecs1, long secs2, long usecs2, long *dsecs, long *dusecs)
{
  *dusecs = (secs2 - secs1)*1000000 + (usecs2 - usecs1);

  *dsecs   = *dusecs / 1000000;
  *dusecs %= 1000000;
}

void
COSTime::
diffHRTime(long secs1, long usecs1, long secs2, long usecs2, long *dusecs)
{
  *dusecs = (secs2 - secs1)*1000000 + (usecs2 - usecs1);
}

void
COSTime::
diffHRTime(long secs1, long usecs1, long secs2, long usecs2, double *dsecs)
{
  long dusecs = (secs2 - secs1)*1000000 + (usecs2 - usecs1);

  *dsecs = dusecs / 100000.0;
}


uint
COSTime::
getElapsedCPU()
{
#if defined(HAS_SYS_TIMES)
  struct tms atms;

  clock_t elapsed = times(&atms);

  return elapsed;
#else
  clock_t elapsed = clock();

  return elapsed;
#endif
}

double
COSTime::
getUserCPU()
{
#if defined(HAS_SYS_TIMES)
  struct tms atms;

  clock_t elapsed = times(&atms);

  if (elapsed <= 0) return 0.0;

  long ticks = sysconf(_SC_CLK_TCK);

  return atms.tms_utime/double(ticks);
#else
  clock_t elapsed = clock();

  return elapsed/double(CLOCKS_PER_SEC);
#endif
}

double
COSTime::
getSystemCPU()
{
#if defined(HAS_SYS_TIMES)
  struct tms atms;

  clock_t elapsed = times(&atms);

  if (elapsed <= 0) return 0.0;

  long ticks = sysconf(_SC_CLK_TCK);

  return atms.tms_stime/double(ticks);
#else
  clock_t elapsed = clock();

  return elapsed/double(CLOCKS_PER_SEC);
#endif
}
