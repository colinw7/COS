#ifndef CHRTime_H
#define CHRTime_H

#include <iostream>

struct CHRTime {
  int secs;
  int usecs;

  CHRTime() : secs(0), usecs(0) { }

  friend std::ostream &operator<<(std::ostream &os, const CHRTime &hrtime) {
    double d = hrtime.secs + hrtime.usecs/1000000.;

    os << d;

    return os;
  }

  double getSecs () { return         secs + usecs/1000000.0; }
  double getMSecs() { return    1000*secs + usecs/1000.0   ; }
  double getUSecs() { return 1000000*secs + usecs          ; }

  const CHRTime &operator+=(const CHRTime &rhs) {
    usecs += rhs.usecs;
    secs  += rhs.secs;

    if (usecs > 1000000) {
      secs  += usecs / 10000000;
      usecs  = usecs % 10000000;
    }

    return *this;
  }

  CHRTime operator+(const CHRTime &rhs) {
    CHRTime t = *this;

    t += rhs;

    return t;
  }
};

#endif
