#include <COSErr.h>
#include <ename/ename.c.inc>

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>

void
COSErr::
exit(const char *format, ...)
{
  va_list argList;

  va_start(argList, format);

  outputError(true, errno, true, format, argList);

  va_end(argList);

  terminate(true);
}

/* Display error message including 'errno' diagnostic, and
   return to caller */
void
COSErr::
message(const char *format, ...)
{
  va_list argList;

  int savedErrno = errno;       /* In case we change it here */

  va_start(argList, format);

  outputError(true, errno, true, format, argList);

  va_end(argList);

  errno = savedErrno;
}

void
COSErr::
outputError(bool useErr, int err, bool flushStdout, const char *format, va_list ap)
{
  enum { BUF_SIZE = 500 };

  char buf[2*BUF_SIZE], userMsg[BUF_SIZE], errText[BUF_SIZE];

  vsnprintf(userMsg, BUF_SIZE, format, ap);

  if (useErr)
    snprintf(errText, BUF_SIZE, " [%s %s]", (err > 0 && err <= MAX_ENAME) ?
             ename[err] : "?UNKNOWN?", strerror(err));
  else
    snprintf(errText, BUF_SIZE, ":");

  snprintf(buf, 2*BUF_SIZE + 32, "ERROR%s %s\n", errText, userMsg);

  if (flushStdout)
    fflush(stdout); /* Flush any pending stdout */

  fputs(buf, stderr);

  fflush(stderr); /* In case stderr is not line-buffered */
}

void
COSErr::
terminate(bool useExit3)
{
  /* Dump core if EF_DUMPCORE environment variable is defined and is a nonempty string;
     otherwise call exit(3) or _exit(2), depending on the value of 'useExit3'. */

  char *s = getenv("EF_DUMPCORE");

  if      (s != NULL && *s != '\0')
    abort();
  else if (useExit3)
    ::exit(EXIT_FAILURE);
  else
    _exit(EXIT_FAILURE);
}
