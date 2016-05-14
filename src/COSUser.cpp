#include <COSUser.h>

#include <std_os.h>

#include <pwd.h>
#include <grp.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/stat.h>

uint
COSUser::
getUserId()
{
  uint uid;

  if (! getUserId(&uid)) {
    setErrorMsg("Invalid User");

    return ~0;
  }

  return uid;
}

bool
COSUser::
getUserId(uint *uid)
{
  *uid = getuid();

  return true;
}

uint
COSUser::
getUserId(const std::string &name)
{
  uint uid;

  if (! getUserId(name, &uid)) {
    setErrorMsg("Invalid User Name: " + name);

    return ~0;
  }

  return uid;
}

bool
COSUser::
getUserId(const std::string &name, uint *uid)
{
#ifdef OS_UNIX
  struct passwd *pw = getpwnam((char *) name.c_str());

  if (pw == NULL)
    return false;

  *uid = pw->pw_uid;

  return true;
#else
  *uid = 0;

  return true;
#endif
}

uint
COSUser::
getEffectiveUserId()
{
  uint uid;

  if (! getEffectiveUserId(&uid)) {
    setErrorMsg("Invalid User");

    return ~0;
  }

  return uid;
}

bool
COSUser::
getEffectiveUserId(uint *uid)
{
  *uid = geteuid();

  return true;
}

bool
COSUser::
isUserName(const std::string &name)
{
  uint uid;

  return getUserId(name, &uid);
}

std::string
COSUser::
getUserName()
{
  std::string name;

  if (! getUserName(name)) {
    setErrorMsg("Invalid User");

    return "";
  }

  return name;
}

bool
COSUser::
getUserName(std::string &name)
{
  uint uid;

  if (! getUserId(&uid))
    return false;

  if (! getUserName(uid, name))
    return false;

  return true;
}

std::string
COSUser::
getEffectiveUserName()
{
  std::string name;

  if (! getEffectiveUserName(name)) {
    setErrorMsg("Invalid User");

    return "";
  }

  return name;
}

bool
COSUser::
getEffectiveUserName(std::string &name)
{
  uint uid;

  if (! getEffectiveUserId(&uid))
    return false;

  if (! getUserName(uid, name))
    return false;

  return true;
}

std::string
COSUser::
getUserName(uint uid)
{
  std::string name;

  if (! getUserName(uid, name))
    return "";

  return name;
}

bool
COSUser::
getUserName(uint uid, std::string &name)
{
#ifdef OS_UNIX
  struct passwd *pw = getpwuid((uid_t) uid);

  if (pw != NULL)
    name = pw->pw_name;
  else {
    char *name1 = getlogin();

    if (name1 != NULL)
      name = name1;
    else {
      char buffer[32];

      sprintf(buffer, "%d", uid);

      name = buffer;
    }
  }
#else
  char buffer[32];

  sprintf(buffer, "%d", uid);

  name = buffer;
#endif

  return true;
}

std::string
COSUser::
getUserDesc()
{
  std::string desc;

  if (! getUserDesc(desc))
    return "";

  return desc;
}

bool
COSUser::
getUserDesc(std::string &desc)
{
  uint uid;

  if (! getUserId(&uid))
    return false;

  if (! getUserDesc(uid, desc))
    return false;

  return true;
}

std::string
COSUser::
getEffectiveUserDesc()
{
  std::string desc;

  if (! getUserDesc(desc))
    return "";

  return desc;
}

bool
COSUser::
getEffectiveUserDesc(std::string &desc)
{
  uint uid;

  if (! getEffectiveUserId(&uid))
    return false;

  if (! getUserDesc(uid, desc))
    return false;

  return true;
}

std::string
COSUser::
getUserDesc(const std::string &name)
{
  std::string desc;

  if (! getUserDesc(name, desc))
    return "";

  return desc;
}

bool
COSUser::
getUserDesc(const std::string &name, std::string &desc)
{
  uint uid;

  if (! getUserId(name, &uid))
    return false;

  if (! getUserDesc(uid, desc))
    return false;

  return true;
}

std::string
COSUser::
getUserDesc(uint uid)
{
  std::string desc;

  if (! getUserDesc(uid, desc))
    return "";

  return desc;
}

bool
COSUser::
getUserDesc(uint uid, std::string &desc)
{
#ifdef OS_UNIX
  struct passwd *pw = getpwuid((uid_t) uid);

  if (pw != NULL) {
    desc = pw->pw_gecos;

    return true;
  }

  return false;
#else
  desc = "";

  return true;
#endif
}

std::string
COSUser::
getUserHome()
{
  std::string home_dir;

  if (! getUserHome(home_dir))
    return "/";

  return home_dir;
}

bool
COSUser::
getUserHome(std::string &home_dir)
{
  uint uid;

  if (! getUserId(&uid))
    return false;

  if (! getUserHome(uid, home_dir)) {
    setErrorMsg("Unknown user");

    return false;
  }

  return true;
}

std::string
COSUser::
getEffectiveUserHome()
{
  std::string home_dir;

  if (! getEffectiveUserHome(home_dir))
    return "/";

  return home_dir;
}

bool
COSUser::
getEffectiveUserHome(std::string &home_dir)
{
  uint uid;

  if (! getEffectiveUserId(&uid))
    return false;

  if (! getUserHome(uid, home_dir)) {
    setErrorMsg("Unknown user");

    return false;
  }

  return true;
}

std::string
COSUser::
getUserHome(const std::string &name)
{
  std::string home_dir;

  if (! getUserHome(name, home_dir)) {
    setErrorMsg("Unknown user name: " + name);

    return "/";
  }

  return home_dir;
}

bool
COSUser::
getUserHome(const std::string &name, std::string &home_dir)
{
  uint uid = getUserId(name);

  if (! getUserHome(uid, home_dir)) {
    setErrorMsg("Unknown user name: " + name);

    return false;
  }

  return true;
}

std::string
COSUser::
getUserHome(uint uid)
{
  std::string home_dir;

  if (! getUserHome(uid, home_dir)) {
    setErrorMsg("Unknown user id: " + uid);

    return "/";
  }

  return home_dir;
}

bool
COSUser::
getUserHome(uint uid, std::string &home_dir)
{
#ifdef OS_UNIX
  struct passwd *pw = getpwuid((uid_t) uid);

  if (pw != NULL) {
    home_dir = pw->pw_dir;

    return true;
  }
  else
    return false;
#else
  char *home_env = getenv("HOME");

  if (home_env != NULL) {
    if (! CFile::isDirectory(home_env))
      home_env = NULL;
  }

  if (home_env == NULL)
    home_env = "/";

  home_dir = home_env;

  if (home_dir[home_dir.size() - 1] != '/')
    home_dir += "/";

  return true;
#endif
}

uint
COSUser::
getUserGroupId()
{
  uint uid;

  if (! getUserId(&uid))
    return ~0;

  return getUserGroupId(uid);
}

uint
COSUser::
getEffectiveUserGroupId()
{
  uint uid;

  if (! getEffectiveUserId(&uid))
    return ~0;

  return getUserGroupId(uid);
}

uint
COSUser::
getUserGroupId(const std::string &name)
{
  return getUserGroupId(getUserId(name));
}

uint
COSUser::
getUserGroupId(uint uid)
{
#ifdef OS_UNIX
  struct passwd *pw = getpwuid((uid_t) uid);

  if (pw != NULL)
    return pw->pw_gid;
  else
    return 0;
#else
  return 0;
#endif
}

std::string
COSUser::
getUserShell()
{
  uint uid;

  if (! getUserId(&uid))
    return "";

  return getUserShell(uid);
}

std::string
COSUser::
getEffectiveUserShell()
{
  uint uid;

  if (! getEffectiveUserId(&uid))
    return "";

  return getUserShell(uid);
}

std::string
COSUser::
getUserShell(const std::string &name)
{
  return getUserShell(getUserId(name));
}

std::string
COSUser::
getUserShell(uint uid)
{
#ifdef OS_UNIX
  struct passwd *pw = getpwuid((uid_t) uid);

  if (pw != NULL)
    return pw->pw_shell;
  else
    return "/bin/sh";
#else
  return "/bin/sh";
#endif
}

uint
COSUser::
getGroupId()
{
  uint gid;

  if (! getGroupId(&gid)) {
    setErrorMsg("Invalid Group");

    return ~0;
  }

  return gid;
}

bool
COSUser::
getGroupId(uint *gid)
{
#ifdef OS_UNIX
  *gid = getegid();
#else
  *gid = 0;
#endif

  return true;
}

uint
COSUser::
getGroupId(const std::string &name)
{
  uint gid;

  if (! getGroupId(name, &gid)) {
    setErrorMsg("Invalid Group Name: " + name);

    return ~0;
  }

  return gid;
}

bool
COSUser::
getGroupId(const std::string &name, uint *gid)
{
#ifdef OS_UNIX
  struct group *gr = getgrnam((char *) name.c_str());

  if (gr == NULL)
    return false;

  *gid = gr->gr_gid;

  return true;
#else
  *gid = 0;

  return true;
#endif
}

bool
COSUser::
isGroupName(const std::string &name)
{
#ifdef OS_UNIX
  struct group *gr = getgrnam((char *) name.c_str());

  return (gr != NULL);
#else
  return false;
#endif
}

std::string
COSUser::
getGroupName()
{
  return getGroupName(getGroupId());
}

std::string
COSUser::
getGroupName(uint gid)
{
#ifdef OS_UNIX
  struct group *gr = getgrgid((gid_t) gid);

  std::string group_name;

  if (gr != NULL)
    group_name = gr->gr_name;
  else {
    char buffer[32];

    sprintf(buffer, "%d", gid);

    group_name = buffer;
  }

  return group_name;
#else
  return "";
#endif
}

uint
COSUser::
getUMask()
{
#ifdef OS_UNIX
  return umask(0);
#else
  return 0;
#endif
}

bool
COSUser::
setUMask(uint mask)
{
#ifdef OS_UNIX
  umask((mode_t) mask);

  return true;
#else
  return false;
#endif
}

std::string
COSUser::
getHostName()
{
  char hostname[256];

  int rc = gethostname(hostname, 256);

  if (rc != 0) {
    setErrorMsg("gethostname failed in COSUser::getHostName");
    return "localhost";
  }

  return hostname;
}

#if 0
bool
COSUser::
matchUsers(const std::string &pattern, std::vector<std::string> &names)
{
#ifdef OS_UNIX
  CGlob glob(pattern);

  glob.setAllowOr(false);
  glob.setAllowNonPrintable(true);

  std::vector<std::string> pw_names;

  getUsers(pw_names);

  int num_names = pw_names.size();

  for (int i = 0; i < num_names; ++i) {
    if (glob.compare(pw_names[i]))
      names.push_back(pw_names[i]);
  }

  if (names.size() == 0)
    return false;

  return true;
#else
  return false;
#endif
}
#endif

bool
COSUser::
getUsers(std::vector<std::string> &names)
{
#ifdef OS_UNIX
  setpwent();

  struct passwd *pwent = getpwent();

  while (pwent != NULL) {
    names.push_back(pwent->pw_name);

    pwent = getpwent();
  }

  endpwent();

  return true;
#else
  return false;
#endif
}

const char *
COSUser::
getShellName()
{
  const char *shell = getenv("SHELL");

  if (shell == NULL || *shell == '\0')
    shell = "/bin/sh";

  return shell;
}

//------

namespace {
static std::string cos_error_msg;
}

const std::string &
COSUser::
getErrorMsg()
{
  return cos_error_msg;
}

void
COSUser::
setErrorMsg(const std::string &str)
{
  cos_error_msg = str;
}
