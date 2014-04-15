#include <COSRead.h>

#include <cerrno>
#include <vector>
#include <poll.h>
#include <unistd.h>

bool
COSRead::
wait_read(int fd, int maxsecs, int maxusecs)
{
  return wait_read(&fd, 1, NULL, maxsecs, maxusecs);
}

bool
COSRead::
wait_read(int *fds, uint num_fds, int maxsecs, int maxusecs)
{
  return wait_read(fds, num_fds, NULL, maxsecs, maxusecs);
}

bool
COSRead::
wait_read(int *fds, uint num_fds, bool *flags, int maxsecs, int maxusecs)
{
  int max_fd = -1;

  for (uint i = 0; i < num_fds; ++i)
    max_fd = std::max(fds[i], max_fd);

  int rc = 0;

  do {
    fd_set fdset, exceptfdset;

    FD_ZERO(&fdset);

    for (uint i = 0; i < num_fds; ++i)
      FD_SET(fds[i], &fdset);

    FD_ZERO(&exceptfdset);

    for (uint i = 0; i < num_fds; ++i)
      FD_SET(fds[i], &exceptfdset);

    struct timeval *ptv = NULL;

    if (maxsecs > 0 || maxusecs > 0) {
      static struct timeval tv;

      tv.tv_sec  = maxsecs;
      tv.tv_usec = maxusecs;

      ptv = &tv;
    }

    rc = select(max_fd + 1, &fdset, NULL, &exceptfdset, ptv);

    if (flags != NULL) {
      for (uint i = 0; i < num_fds; ++i)
        flags[i] = FD_ISSET(fds[i], &fdset);
    }
  }
  while (rc == -1 && errno == EINTR);

  if (rc == 0) {
#if 0
    cerr << "wait_read timed out" << endl;
#endif
    errno = ETIMEDOUT;
    return false;
  }

  return (rc > 0);
}

bool
COSRead::
wait_write(int fd, int maxsecs, int maxusecs)
{
  return wait_write(&fd, 1, NULL, maxsecs, maxusecs);
}

bool
COSRead::
wait_write(int *fds, uint num_fds, int maxsecs, int maxusecs)
{
  return wait_write(fds, num_fds, NULL, maxsecs, maxusecs);
}

bool
COSRead::
wait_write(int *fds, uint num_fds, bool *flags, int maxsecs, int maxusecs)
{
  int max_fd = -1;

  for (uint i = 0; i < num_fds; ++i)
    max_fd = std::max(fds[i], max_fd);

  int rc = 0;

  do {
    fd_set fdset, exceptfdset;

    FD_ZERO(&fdset);

    for (uint i = 0; i < num_fds; ++i)
      FD_SET(fds[i], &fdset);

    FD_ZERO(&exceptfdset);

    for (uint i = 0; i < num_fds; ++i)
      FD_SET(fds[i], &exceptfdset);

    struct timeval *ptv = NULL;

    if (maxsecs > 0 || maxusecs > 0) {
      static struct timeval tv;

      tv.tv_sec  = maxsecs;
      tv.tv_usec = maxusecs;

      ptv = &tv;
    }

    rc = select(max_fd + 1, NULL, &fdset, &exceptfdset, ptv);

    if (flags != NULL) {
      for (uint i = 0; i < num_fds; ++i)
        flags[i] = FD_ISSET(fds[i], &fdset);
    }
  }
  while (rc == -1 && errno == EINTR);

  if (rc == 0) {
#if 0
    cerr << "wait_write timed out" << endl;
#endif
    errno = ETIMEDOUT;
    return false;
  }

  return (rc > 0);
}

bool
COSRead::
read(int fd, std::string &str)
{
  static char buffer[512];

  str = "";

  ssize_t m = sizeof(buffer) - 1;

  while (true) {
    ssize_t n = ::read(fd, buffer, m);

    if (n == -1) {
      if (errno == EINTR)
        continue;
      else
        return false;
    }

    buffer[n] = '\0';

    str += buffer;

    if (n < m)
      break;

    if (! wait_read(fd, 0, 10))
      break;
  }

  return true;
}

// read n bytes from file (fd) into buffer
ssize_t
readall(int fd, void *buffer, size_t n)
{
  char *buf = (char *) buffer;

  size_t totRead = 0;

  while (totRead < n) {
    ssize_t numRead = read(fd, buf, n - totRead);

    if (numRead == 0) /* EOF */
      return totRead; /* May be 0 if this is first read() */

    if (numRead == -1) {
      if (errno == EINTR)
        continue;               /* Interrupted --> restart read() */
      else
        return -1;              /* Some other error */
    }

    totRead += numRead;
    buf     += numRead;
  }

  return totRead; /* Must be 'n' bytes if we get here */
}

bool
COSRead::
write(int fd, char c)
{
  ssize_t n = writeall(fd, &c, 1);

  return (n == 1);
}

bool
COSRead::
write(int fd, const std::string &str)
{
  ssize_t n = writeall(fd, str.c_str(), str.size());

  return (n == (ssize_t) str.size());
}

ssize_t
COSRead::
writeall(int fd, const void *buf, size_t count_in)
{
  const char *buf1 = (const char *) buf;

  size_t count_out = 0;

  while (count_out < count_in) {
    ssize_t n = ::write(fd, buf1, count_in - count_out);

    /* The "write() returns 0" case should never happen, but the
           following ensures that we don't loop forever if it does */
    if (n <= 0) {
      if (n == -1 && errno == EINTR)
        continue; /* Interrupted --> restart write() */
      else
        return -1; /* Some other error */
    }

    count_out += n;
    buf1      += n;
  }

  return count_out;
}

//---------------

bool
COSRead::
poll_read(int *fds, uint num_fds, bool *flags, int max_msecs)
{
  static std::vector<struct pollfd> fdArray;

  fdArray.resize(num_fds);

  for (uint i = 0; i < num_fds; ++i) {
    fdArray[i].fd     = fds[i];
    fdArray[i].events = POLLIN;
  }

  int rc = 0;

  do {
    rc = poll(&fdArray[0], num_fds, max_msecs);

    if (flags != NULL) {
      for (uint i = 0; i < num_fds; ++i)
        flags[i] = (fdArray[i].revents & POLLIN);
    }
  }
  while (rc == -1 && errno == EINTR);

  if (rc == 0) {
#if 0
    cerr << "timed out" << endl;
#endif
    errno = ETIMEDOUT;
    return false;
  }

  return (rc > 0);
}

bool
COSRead::
poll_write(int *fds, uint num_fds, bool *flags, int max_msecs)
{
  static std::vector<struct pollfd> fdArray;

  fdArray.resize(num_fds);

  for (uint i = 0; i < num_fds; ++i) {
    fdArray[i].fd     = fds[i];
    fdArray[i].events = POLLOUT;
  }

  int rc = 0;

  do {
    rc = poll(&fdArray[0], num_fds, max_msecs);

    if (flags != NULL) {
      for (uint i = 0; i < num_fds; ++i)
        flags[i] = (fdArray[i].revents & POLLOUT);
    }
  }
  while (rc == -1 && errno == EINTR);

  if (rc == 0) {
#if 0
    cerr << "timed out" << endl;
#endif
    errno = ETIMEDOUT;
    return false;
  }

  return (rc > 0);
}
