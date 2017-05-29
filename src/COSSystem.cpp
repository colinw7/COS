#include <COSSystem.h>

#include <std_os.h>
#include <sys/utsname.h>

uint
COSSystem::
getWordSize()
{
  return sizeof(int);
}

std::string
COSSystem::
getSystemName()
{
  struct utsname sys;

  uname(&sys);

  return sys.sysname;
}

std::string
COSSystem::
getSystemRelease()
{
  struct utsname sys;

  uname(&sys);

  return sys.release;
}

std::string
COSSystem::
getSystemMachine()
{
  struct utsname sys;

  uname(&sys);

  return sys.machine;
}
