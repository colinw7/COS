#include <COSTimer.h>

#include <std_os.h>

#include <time.h>
#include <cerrno>

#define HAS_NANO_SLEEP 1

uint
COSTimer::
sleep(uint secs)
{
#if OS_UNIX
  // TODO: use nanosleep or clock_nanosleep (linux)

  int remain = sleep((uint) secs);

  return remain;
#else
  std::cerr << "COSTimer::sleep: Unimplemented" << std::endl;

  return 0;
#endif
}

uint
COSTimer::
msleep(uint msecs)
{
  return micro_sleep(msecs);
}

// sleep for n millionths of a second
uint
COSTimer::
micro_sleep(uint msecs)
{
#if HAS_NANO_SLEEP
  struct timespec tv;

  tv.tv_sec  = msecs / 1000000;
  tv.tv_nsec = 1000*(msecs % 1000000);

  (void) nanosleep(&tv, NULL);

  return 0;
#elif OS_UNIX
  if (msecs <= 0)
    return 0;

  struct timeval tv;

  tv.tv_sec  = msecs / 1000000;
  tv.tv_usec = msecs % 1000000;

  select(1, 0, 0, 0, &tv);

  return 0;
#else
  std::cerr << "COSTimer::micro_sleep: Unimplemented" << std::endl;

  return 0;
#endif
}

// sleep for n thousanths of a second
uint
COSTimer::
milli_sleep(uint msecs)
{
#if HAS_NANO_SLEEP
  struct timespec tv;

  tv.tv_sec  = msecs / 1000;
  tv.tv_nsec = 1000000*(msecs % 1000);

  (void) nanosleep(&tv, NULL);

  return 0;
#elif OS_UNIX
  if (msecs <= 0)
    return 0;

  struct timeval tv;

  tv.tv_sec  = msecs / 1000;
  tv.tv_usec = 1000*(msecs % 1000);

  select(1, 0, 0, 0, &tv);

  return 0;
#else
  std::cerr << "COSTimer::milli_sleep: Unimplemented" << std::endl;

  return 0;
#endif
}
