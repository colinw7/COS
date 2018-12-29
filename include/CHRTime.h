#ifndef CHRTime_H
#define CHRTime_H

#include <sys/time.h>
#include <iostream>

struct CHRTime {
  long secs  { 0 };
  long usecs { 0 };

  CHRTime(long secs=0, long usecs=0) :
   secs(secs), usecs(usecs) {
  }

  friend std::ostream &operator<<(std::ostream &os, const CHRTime &hrtime) {
    os << hrtime.getSecs();

    return os;
  }

  double getSecs () const { return         secs + usecs/1000000.0; }
  double getMSecs() const { return    1000*secs + usecs/1000.0   ; }
  double getUSecs() const { return 1000000*secs + usecs          ; }

  void setSecs(double t) {
    secs  = t;
    usecs = 0;
  }

  void setMSecs(double t) {
    secs  = 0;
    usecs = t*1000;

    normalize();
  }

  void setUSecs(double t) {
    secs  = 0;
    usecs = t;

    normalize();
  }

  const CHRTime &operator+=(const CHRTime &rhs) {
    usecs += rhs.usecs;
    secs  += rhs.secs;

    normalize();

    return *this;
  }

  CHRTime operator+(const CHRTime &rhs) const {
    CHRTime t = *this;

    t += rhs;

    return t;
  }

  const CHRTime &operator-=(const CHRTime &rhs) {
    usecs -= rhs.usecs;
    secs  -= rhs.secs;

    normalize();

    return *this;
  }

  CHRTime operator-(const CHRTime &rhs) const {
    CHRTime t = *this;

    t -= rhs;

    return t;
  }

  void normalize() {
    if      (usecs < 0) {
      while (usecs < 0) {
        usecs += 1000000;

        --secs;
      }
    }
    else if (usecs > 1000000) {
      secs  += usecs / 1000000;
      usecs  = usecs % 1000000;
    }
  }

  bool isSet() const { return secs > 0 || usecs > 0; }

  CHRTime diffTime(const CHRTime &hrtime2=getTime()) const {
    return diffTime(*this, hrtime2);
  }

  static CHRTime getTime() {
    CHRTime hrtime;

    struct timeval timeval;

    gettimeofday(&timeval, nullptr);

    hrtime.secs  = timeval.tv_sec;
    hrtime.usecs = timeval.tv_usec;

    return hrtime;
  }

  static CHRTime diffTime(const CHRTime &hrtime1, const CHRTime &hrtime2) {
    CHRTime hrtime;

    hrtime.usecs = (hrtime2.secs - hrtime1.secs)*1000000 + (hrtime2.usecs - hrtime1.usecs);

    hrtime.normalize();

    return hrtime;
  }

  static int cmp(const CHRTime &hrtime1, const CHRTime &hrtime2) {
    if (! hrtime1.isSet() && ! hrtime2.isSet()) return 0;

    if (! hrtime1.isSet()) return -1;
    if (! hrtime2.isSet()) return  1;

    if (hrtime1.secs > hrtime2.secs) return  1;
    if (hrtime1.secs < hrtime2.secs) return -1;

    if (hrtime1.usecs > hrtime2.usecs) return  1;
    if (hrtime1.usecs < hrtime2.usecs) return -1;

    return 0;
  }

  friend bool operator==(const CHRTime &hrtime1, const CHRTime &hrtime2) {
    return cmp(hrtime1, hrtime2) == 0;
  }

  friend bool operator!=(const CHRTime &hrtime1, const CHRTime &hrtime2) {
    return cmp(hrtime1, hrtime2) != 0;
  }

  friend bool operator>(const CHRTime &hrtime1, const CHRTime &hrtime2) {
    return cmp(hrtime1, hrtime2) > 0;
  }

  friend bool operator>=(const CHRTime &hrtime1, const CHRTime &hrtime2) {
    return cmp(hrtime1, hrtime2) >= 0;
  }

  friend bool operator<(const CHRTime &hrtime1, const CHRTime &hrtime2) {
    return cmp(hrtime1, hrtime2) < 0;
  }

  friend bool operator<=(const CHRTime &hrtime1, const CHRTime &hrtime2) {
    return cmp(hrtime1, hrtime2) <= 0;
  }
};

#endif
