#include <COSFile.h>

#include <std_os.h>
#include <sys/param.h>

#include <cerrno>
#include <fcntl.h>
#include <cstring>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <dirent.h>
#include <unistd.h>

#ifdef OS_UNIX
#include <libgen.h>
#endif

std::string
COSFile::
getCurrentDir()
{
  std::string dirname;

  if (! getCurrentDir(dirname))
    return "";

  return dirname;
}

bool
COSFile::
getCurrentDir(std::string &dirname)
{
  char dirname1[MAXPATHLEN];

#ifdef OS_UNIX
  getcwd(dirname1, MAXPATHLEN);
#else
  _getcwd(dirname1, MAXPATHLEN);
#endif

  dirname = dirname1;

  return true;
}

//------

bool
COSFile::
changeDir(const std::string &dirname)
{
#ifdef OS_UNIX
  int error = chdir(dirname.c_str());
#else
  int error = _chdir(dirname.c_str());
#endif

  if (error != 0) {
    setErrorMsg(dirname + ": Failed to Change to Directory");

    return false;
  }

  return true;
}

void *
COSFile::
openDir(const std::string &dirname)
{
#ifdef OS_UNIX
  DIR *dir = opendir(dirname.c_str());

  if (dir == NULL) {
    if      (errno == ENOENT)
      setErrorMsg("Directory does not exist");
    else if (errno == EACCES)
      setErrorMsg("Failed to Read Directory");
    else if (errno == ENOTDIR)
      setErrorMsg("CFile is not a Directory");
    else
      setErrorMsg("Failed to Open Directory");
  }

  return (void *) dir;
#else
  int handle = -1;

  return (void *) handle;
#endif
}

bool
COSFile::
readDir(void *handle, std::string &filename)
{
#ifdef OS_UNIX
  DIR *dir = (DIR *) handle;

  if (dir == NULL)
    return false;

  struct dirent *dirent = readdir(dir);

  if (dirent == NULL)
    return false;

  while (dirent->d_name[0] == '.' &&
         (dirent->d_name[1] == '\0' ||
          (dirent->d_name[1] == '.' && dirent->d_name[2] == '\0'))) {
    dirent = readdir(dir);

    if (dirent == NULL)
      return false;
  }

  filename = dirent->d_name;

  return true;
#else
  int handle1 = (int) handle;

  struct _finddata_t find_data;

  if (handle1 == -1) {
    handle1 = _findfirst("*", &find_data);

    if (handle1 == -1) {
      setErrorMsg("Failed to Read Directory");

      return false;
    }
  }

  while (find_data.name[0] == '.' &&
         (find_data.name[1] == '\0' ||
          (find_data.name[1] == '.' && find_data.name[2] == '\0'))) {
    int error = _findnext(handle1, &find_data);

    if (error == -1)
      return false;
  }

  filename = find_data.name;

  return true;
#endif
}

bool
COSFile::
closeDir(void *handle)
{
#ifdef OS_UNIX
  DIR *dir = (DIR *) handle;

  closedir(dir);

  return true;
#else
  return true;
#endif
}

bool
COSFile::
getTempFileName(std::string &filename)
{
  return getTempFileName("", filename);
}

bool
COSFile::
getTempFileName(const std::string &dir, std::string &filename)
{
#ifdef OS_LINUX
  static char filename1[MAXPATHLEN];

  int fd = -1;

  if (dir != "") {
    strcpy(filename1, dir.c_str());
    strcat(filename1, "/fileXXXXXX");

    fd = mkstemp(filename1);
  }

  if (fd < 0) {
    strcpy(filename1, "/tmp/fileXXXXXX");

    fd = mkstemp(filename1);
  }

  if (fd < 0) {
    strcpy(filename1, "/usr/tmp/fileXXXXXX");

    fd = mkstemp(filename1);
  }

  if (fd < 0)
    return false;

  ::close(fd);

  filename = filename1;
#else
  if (dir != "")
    filename = tempnam((char *) dir.c_str(), NULL);
  else
    filename = tempnam(NULL, NULL);
#endif

  return true;
}

int
COSFile::
getTempFileNum(std::string &filename, bool del_on_close)
{
  return getTempFileNum("", filename, del_on_close);
}

int
COSFile::
getTempFileNum(const std::string &dir, std::string &filename, bool del_on_close)
{
#ifdef OS_LINUX
  static char filename1[MAXPATHLEN];

  int fd = -1;

  if (dir != "") {
    strcpy(filename1, dir.c_str());
    strcat(filename1, "/fileXXXXXX");

    fd = mkstemp(filename1);
  }

  if (fd < 0) {
    strcpy(filename1, "/tmp/fileXXXXXX");

    fd = mkstemp(filename1);
  }

  if (fd < 0) {
    strcpy(filename1, "/usr/tmp/fileXXXXXX");

    fd = mkstemp(filename1);
  }

  if (del_on_close)
    unlink(filename1);

  filename = filename1;

  return fd;
#else
  if (dir != "")
    filename = tempnam((char *) dir.c_str(), NULL);
  else
    filename = tempnam(NULL, NULL);

  int fd = creat(filename.c_str(), 0666);

  if (del_on_close)
    unlink(filename.c_str());

  return fd;
#endif
}

//------

int
COSFile::
lstat(const char *filename, struct stat *file_stat)
{
#ifdef OS_UNIX
  return stat(filename, file_stat);
#else
  return stat(filename, file_stat);
#endif
}

bool
COSFile::
stat_is_reg(const struct stat *file_stat)
{
  return stat_mode_is_reg(file_stat->st_mode);
}

bool
COSFile::
stat_mode_is_reg(uint mode)
{
  return S_ISREG(mode);
}

bool
COSFile::
stat_is_dir(const struct stat *file_stat)
{
  return stat_mode_is_dir(file_stat->st_mode);
}

bool
COSFile::
stat_mode_is_dir(uint mode)
{
  return S_ISDIR(mode);
}

bool
COSFile::
stat_is_block(const struct stat *file_stat)
{
  return stat_mode_is_block(file_stat->st_mode);
}

bool
COSFile::
stat_mode_is_block(uint mode)
{
#ifdef OS_UNIX
  return S_ISBLK(mode);
#else
  return false;
#endif
}

bool
COSFile::
stat_is_char(const struct stat *file_stat)
{
  return stat_mode_is_char(file_stat->st_mode);
}

bool
COSFile::
stat_mode_is_char(uint mode)
{
#ifdef OS_UNIX
  return S_ISCHR(mode);
#else
  return false;
#endif
}

bool
COSFile::
stat_is_link(const struct stat *file_stat)
{
  return stat_mode_is_link(file_stat->st_mode);
}

bool
COSFile::
stat_mode_is_link(uint mode)
{
#ifdef OS_UNIX
  return S_ISLNK(mode);
#else
  return false;
#endif
}

bool
COSFile::
stat_is_fifo(const struct stat *file_stat)
{
  return stat_mode_is_fifo(file_stat->st_mode);
}

bool
COSFile::
stat_mode_is_fifo(uint mode)
{
#ifdef OS_UNIX
  return S_ISFIFO(mode);
#else
  return false;
#endif
}

bool
COSFile::
stat_is_socket(const struct stat *file_stat)
{
  return stat_mode_is_socket(file_stat->st_mode);
}

bool
COSFile::
stat_mode_is_socket(uint mode)
{
#ifdef OS_UNIX
  return S_ISSOCK(mode);
#else
  return false;
#endif
}

bool
COSFile::
stat_is_uid_on_exec(const struct stat *file_stat)
{
  return stat_mode_is_uid_on_exec(file_stat->st_mode);
}

bool
COSFile::
stat_mode_is_uid_on_exec(uint mode)
{
#ifdef OS_UNIX
  return ((mode & S_ISUID) == S_ISUID);
#else
  return false;
#endif
}

bool
COSFile::
stat_is_gid_on_exec(const struct stat *file_stat)
{
  return stat_mode_is_gid_on_exec(file_stat->st_mode);
}

bool
COSFile::
stat_mode_is_gid_on_exec(uint mode)
{
#ifdef OS_UNIX
  return ((mode & S_ISGID) == S_ISGID);
#else
  return false;
#endif
}

bool
COSFile::
stat_is_dir_restrict_delete(const struct stat *file_stat)
{
  return stat_mode_is_restrict_delete(file_stat->st_mode);
}

bool
COSFile::
stat_mode_is_restrict_delete(uint mode)
{
#ifdef OS_UNIX
  return ((mode & S_ISVTX) == S_ISVTX);
#else
  return false;
#endif
}

bool
COSFile::
stat_is_owner(const struct stat *file_stat)
{
#ifdef OS_UNIX
  return (file_stat->st_uid == getuid());
#else
  return false;
#endif
}

bool
COSFile::
stat_is_effective_owner(const struct stat *file_stat)
{
#ifdef OS_UNIX
  return (file_stat->st_uid == geteuid());
#else
  return false;
#endif
}

bool
COSFile::
stat_is_owner_read(const struct stat *file_stat)
{
  if (! stat_is_owner(file_stat))
    return false;

#ifdef OS_UNIX
  return (file_stat->st_mode & S_IRUSR);
#else
  return false;
#endif
}

bool
COSFile::
stat_is_owner_write(const struct stat *file_stat)
{
  if (! stat_is_owner(file_stat))
    return false;

#ifdef OS_UNIX
  return (file_stat->st_mode & S_IWUSR);
#else
  return false;
#endif
}

bool
COSFile::
stat_is_owner_execute(const struct stat *file_stat)
{
  if (! stat_is_owner(file_stat))
    return false;

#ifdef OS_UNIX
  return (file_stat->st_mode & S_IXUSR);
#else
  return false;
#endif
}

bool
COSFile::
stat_is_effective_owner_read(const struct stat *file_stat)
{
  if (! stat_is_effective_owner(file_stat))
    return false;

#ifdef OS_UNIX
  return (file_stat->st_mode & S_IRUSR);
#else
  return false;
#endif
}

bool
COSFile::
stat_is_effective_owner_write(const struct stat *file_stat)
{
  if (! stat_is_effective_owner(file_stat))
    return false;

#ifdef OS_UNIX
  return (file_stat->st_mode & S_IWUSR);
#else
  return false;
#endif
}

bool
COSFile::
stat_is_effective_owner_execute(const struct stat *file_stat)
{
  if (! stat_is_effective_owner(file_stat))
    return false;

#ifdef OS_UNIX
  return (file_stat->st_mode & S_IXUSR);
#else
  return false;
#endif
}

bool
COSFile::
makeDir(const std::string &dirname, int mode)
{
#ifdef OS_UNIX
  int error = mkdir(dirname.c_str(), (mode_t) mode);
#else
  int error = mkdir(dirname.c_str());
#endif

  if (error != 0) {
    switch (errno) {
      case EPERM:
      case EACCES:
        setErrorMsg("No permission to make directory " + dirname);
        break;
      case EEXIST:
        setErrorMsg("Directory " + dirname + "already exists");
        break;
      case ENAMETOOLONG:
        setErrorMsg("Directory " + dirname + "too long exists");
        break;
      case ENOENT:
      case ENOTDIR:
        setErrorMsg("Directory " + dirname + "has incorrect path");
        break;
      default:
        setErrorMsg("Failed to make directory " + dirname);
        break;
    }

    return false;
  }

  return true;
}

bool
COSFile::
linkFile(const std::string &oldname, const std::string &newname)
{
#ifdef OS_UNIX
  int error = link(oldname.c_str(), newname.c_str());

  if (error != 0)
    return false;

  return true;
#else
  std::cerr << "COSFile::linkFile: Unimplemented" << std::endl;

  return false;
#endif
}

bool
COSFile::
symlinkFile(const std::string &oldname, const std::string &newname)
{
#ifdef OS_UNIX
  int error = symlink(oldname.c_str(), newname.c_str());

  if (error != 0)
    return false;

  return true;
#else
  std::cerr << "COSFile::symlinkFile: Unimplemented" << std::endl;

  return false;
#endif
}

bool
COSFile::
getLinkedFile(const std::string &filename, std::string &linked_filename)
{
#ifdef OS_UNIX
  char buffer[MAXPATHLEN + 1];

  buffer[0] = '\0';

  int len = readlink(filename.c_str(), buffer, MAXPATHLEN);

  if (len >= 0)
    buffer[len] = '\0';
  else
    buffer[0] = '\0';

  linked_filename = buffer;

  if (len < 0)
    return false;

  return true;
#else
  std::cerr << "COSFile::getLinkedFile: Unimplemented" << std::endl;

  return false;
#endif
}

bool
COSFile::
createFile(const std::string &filename, int mode, int *fd)
{
#ifdef OS_UNIX
  *fd = creat(filename.c_str(), (mode_t) mode);
#else
  *fd = creat(filename.c_str(), mode);
#endif

  if (*fd == -1)
    return false;

  return true;
}

bool
COSFile::
chmodFile(const std::string &filename, int mode)
{
#ifdef OS_UNIX
  int error = chmod(filename.c_str(), (mode_t) mode);

  if (error != 0)
    return false;

  return true;
#else
  std::cerr << "COSFile::chmodFile: Unimplemented" << std::endl;

  return false;
#endif
}

bool
COSFile::
chownFile(const std::string &filename, int uid, int gid)
{
#ifdef OS_UNIX
  int error = chown(filename.c_str(), (uid_t) uid, (gid_t) gid);

  if (error != 0)
    return false;

  return true;
#else
  std::cerr << "COSFile::chownFile: Unimplemented" << std::endl;

  return false;
#endif
}

bool
COSFile::
fileCntrl(int fd, int cmd, void *arg)
{
#ifdef OS_UNIX
  switch ((int) cmd) {
    case F_DUPFD: {
      int *arg1 = (int *) arg;

      *arg1 = ::fcntl((int) fd, (int) cmd, *arg1);

      if (*arg1 == -1)
        return false;

      break;
    }
    case F_GETFD:
    case F_GETFL:
#ifdef F_GETOWN
    case F_GETOWN: {
#endif
      int *arg1 = (int *) arg;

      *arg1 = ::fcntl((int) fd, (int) cmd);

      if (*arg1 == -1)
        return false;

      break;
    }
    case F_SETFD:
    case F_SETFL:
#ifdef F_SETOWN
    case F_SETOWN: {
#endif
      int error = ::fcntl((int) fd, (int) cmd, arg);

      if (error == -1)
        return false;

      break;
    }
    case F_SETLK:
    case F_SETLKW:
    case F_GETLK: {
      int error = ::fcntl((int) fd, (int) cmd, (struct flock *) arg);

      if (error == -1)
        return false;

      break;
    }
    default:
      return false;

      break;
  }

  return true;
#else
  std::cerr << "COSFile::fileCntrl: Unimplemented" << std::endl;

  return false;
#endif
}

bool
COSFile::
setFileNonBlocking(int fd, bool nonblock)
{
  int flags = ::fcntl(fd, F_GETFL);

  if (flags == -1)
    return false;

  if (nonblock)
    flags |= O_NONBLOCK;
  else
    flags &= ~O_NONBLOCK;

  int error = ::fcntl(fd, F_SETFL, flags);

  return (error != -1);
}

bool
COSFile::
resolvePath(const std::string &path, std::string &rpath)
{
#ifdef OS_UNIX
  char buffer[PATH_MAX + 1];

  if (realpath(path.c_str(), buffer) == NULL)
    return false;

  rpath = buffer;

  return true;
#else
  std::cerr << "COSFile::resolvePath: Unimplemented" << std::endl;

  return false;
#endif
}

bool
COSFile::
splitPath(const std::string &path, std::string &dir, std::string &base)
{
#ifdef OS_UNIX
  char buffer[PATH_MAX + 1];

  if (path.size() > PATH_MAX) return false;

  strcpy(buffer, path.c_str());

  dir = ::dirname(buffer);

  strcpy(buffer, path.c_str());

  base = ::basename(buffer);

  return true;
#else
  std::cerr << "COSFile::splitPath: Unimplemented" << std::endl;

  return false;
#endif
}

//------

uint
COSFile::
maxFileNameLength()
{
  char dirname[MAXPATHLEN];

#ifdef OS_UNIX
  getcwd(dirname, MAXPATHLEN);
#else
  _getcwd(dirname, MAXPATHLEN);
#endif

  return pathconf(dirname, _PC_NAME_MAX);
}

//------

namespace {
static std::string cos_error_msg;
}

const std::string &
COSFile::
getErrorMsg()
{
  return cos_error_msg;
}

void
COSFile::
setErrorMsg(const std::string &str)
{
  cos_error_msg = str;
}
