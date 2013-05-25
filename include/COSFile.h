#ifndef COSFile_H
#define COSFile_H

#include <sys/types.h>
#include <sys/stat.h>

#include <string>

namespace COSFile {
  std::string getCurrentDir();
  bool        getCurrentDir(std::string &dirname);

  bool changeDir(const std::string &dirname);

  void *openDir(const std::string &dirname);
  bool  readDir(void *handle, std::string &filename);
  bool  closeDir(void *handle);

  bool getTempFileName(std::string &filename);
  bool getTempFileName(const std::string &dir, std::string &filename);

  int getTempFileNum(std::string &filename, bool del_on_close=false);
  int getTempFileNum(const std::string &dir, std::string &filename, bool del_on_close=false);

  int lstat(const char *filename, struct stat *file_stat);

  bool stat_is_reg(const struct stat *file_stat);
  bool stat_mode_is_reg(uint mode);

  bool stat_is_dir(const struct stat *file_stat);
  bool stat_mode_is_dir(uint mode);

  bool stat_is_block(const struct stat *file_stat);
  bool stat_mode_is_block(uint mode);

  bool stat_is_char(const struct stat *file_stat);
  bool stat_mode_is_char(uint mode);

  bool stat_is_link(const struct stat *file_stat);
  bool stat_mode_is_link(uint mode);

  bool stat_is_fifo(const struct stat *file_stat);
  bool stat_mode_is_fifo(uint mode);

  bool stat_is_socket(const struct stat *file_stat);
  bool stat_mode_is_socket(uint mode);

  bool stat_is_uid_on_exec(const struct stat *file_stat);
  bool stat_mode_is_uid_on_exec(uint mode);

  bool stat_is_gid_on_exec(const struct stat *file_stat);
  bool stat_mode_is_gid_on_exec(uint mode);

  bool stat_is_dir_restrict_delete(const struct stat *file_stat);
  bool stat_mode_is_restrict_delete(uint mode);

  bool stat_is_owner(const struct stat *file_stat);
  bool stat_is_effective_owner(const struct stat *file_stat);

  bool stat_is_owner_read(const struct stat *file_stat);
  bool stat_is_owner_write(const struct stat *file_stat);
  bool stat_is_owner_execute(const struct stat *file_stat);

  bool stat_is_effective_owner_read(const struct stat *file_stat);
  bool stat_is_effective_owner_write(const struct stat *file_stat);
  bool stat_is_effective_owner_execute(const struct stat *file_stat);

  bool makeDir(const std::string &dirname, int mode=0777);

  bool linkFile(const std::string &oldname, const std::string &newname);

  bool symlinkFile(const std::string &oldname, const std::string &newname);

  bool getLinkedFile(const std::string &filename, std::string &linked_filename);

  bool createFile(const std::string &filename, int mode, int *fd);

  bool chmodFile(const std::string &filename, int mode);
  bool chownFile(const std::string &filename, int uid, int gid);

  bool fileCntrl(int fd, int cmd, void *arg);

  bool setFileNonBlocking(int fd, bool nonblock=true);

  bool resolvePath(const std::string &path, std::string &rpath);

  bool splitPath(const std::string &path, std::string &dir, std::string &base);

  uint maxFileNameLength();

  const std::string &getErrorMsg();
  void setErrorMsg(const std::string &str);
}

#endif
