#ifndef COSRead_H
#define COSRead_H

#include <sys/types.h>
#include <string>

namespace COSRead {
  bool wait_read (int fd, int maxsecs=0, int maxusecs=0);
  bool wait_read (int *fds, uint num_fds, int maxsecs=0, int maxusecs=0);
  bool wait_read (int *fds, uint num_fds, bool *flags=NULL, int maxsecs=0, int maxusecs=0);

  bool wait_write(int fd, int maxsecs = 0, int maxusecs=0);
  bool wait_write(int *fds, uint num_fds, int maxsecs=0, int maxusecs=0);
  bool wait_write(int *fds, uint num_fds, bool *flags=NULL, int maxsecs=0, int maxusecs=0);

  bool read(int fd, std::string &str);

  bool readall(int fd, void *buf, size_t n);

  bool write(int fd, char c);
  bool write(int fd, const std::string &str);

  ssize_t writeall(int fd, const void *buf, size_t n);

  bool poll_read (int *fds, uint num_fds, bool *flags, int max_msecs);
  bool poll_write(int *fds, uint num_fds, bool *flags, int max_msecs);
}

#endif
