#define FUSE_USE_VERSION 30

#include <fuse.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/xattr.h>

#include <string.h>

static int xmp_getattr(const char *path, struct stat *stbuf) {
  int res;
  res = lstat(path, stbuf);
  if (res == -1)
    return -errno;
  return 0;
}

static int xmp_access(const char *path, int mask) {
  int res;
  res = access(path, mask);
  if (res == -1)
    return -errno;
  return 0;
}

static int xmp_readlink(const char *path, char *buf, size_t size) {
  int res;
  res = readlink(path, buf, size - 1);
  if (res == -1)
    return -errno;
  buf[res] = '\0';
  return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                       off_t offset, struct fuse_file_info *fi) {
  DIR *dp;
  struct dirent *de;
  (void) offset;
  (void) fi;
  dp = opendir(path);
  if (dp == NULL)
    return -errno;
  while ((de = readdir(dp)) != NULL) {
    struct stat st;
    memset(&st, 0, sizeof(st));
    st.st_ino = de->d_ino;
    st.st_mode = de->d_type << 12;
    if (filler(buf, de->d_name, &st, 0))
      break;
  }
  closedir(dp);
  return 0;
}
static int xmp_mknod(const char *path, mode_t mode, dev_t rdev) {
  int res;
  /* On Linux this could just be 'mknod(path, mode, rdev)' but this
     is more portable */
  if (S_ISREG(mode)) {
    res = open(path, O_CREAT | O_EXCL | O_WRONLY, mode);
    if (res >= 0)
      res = close(res);
  } else if (S_ISFIFO(mode))
    res = mkfifo(path, mode);
  else
    res = mknod(path, mode, rdev);
  if (res == -1)
    return -errno;
  return 0;
}
static int xmp_mkdir(const char *path, mode_t mode) {
  int res;
  res = mkdir(path, mode);
  if (res == -1)
    return -errno;
  return 0;
}
static int xmp_unlink(const char *path) {
  int res;
  res = unlink(path);
  if (res == -1)
    return -errno;
  return 0;
}
static int xmp_rmdir(const char *path) {
  //    int res;
  //    res = rmdir(path);
  //    if (res == -1)
  //        return -errno;
  return 0;
}
static int xmp_symlink(const char *from, const char *to) {
  int res;
  res = symlink(from, to);
  if (res == -1)
    return -errno;
  return 0;
}
static int xmp_rename(const char *from, const char *to) {
  int res;
  res = rename(from, to);
  if (res == -1)
    return -errno;
  return 0;
}
static int xmp_link(const char *from, const char *to) {
  int res;
  res = link(from, to);
  if (res == -1)
    return -errno;
  return 0;
}
static int xmp_chmod(const char *path, mode_t mode) {
  int res;
  res = chmod(path, mode);
  if (res == -1)
    return -errno;
  return 0;
}
static int xmp_chown(const char *path, uid_t uid, gid_t gid) {
  int res;
  res = lchown(path, uid, gid);
  if (res == -1)
    return -errno;
  return 0;
}
static int xmp_truncate(const char *path, off_t size) {
  int res;
  res = truncate(path, size);
  if (res == -1)
    return -errno;
  return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi) {
  int res;
  res = open(path, fi->flags);
  if (res == -1)
    return -errno;
  close(res);
  return 0;
}
static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
                    struct fuse_file_info *fi) {
  int fd;
  int res;
  (void) fi;
  fd = open(path, O_RDONLY);
  if (fd == -1)
    return -errno;
  res = pread(fd, buf, size, offset);
  if (res == -1)
    res = -errno;
  close(fd);
  return res;
}

static int xmp_write(const char *path, const char *buf, size_t size,
                     off_t offset, struct fuse_file_info *fi) {
  return 0;
}

static int xmp_statfs(const char *path, struct statvfs *stbuf) {
  int res;
  res = statvfs(path, stbuf);
  if (res == -1)
    return -errno;
  return 0;
}
static int xmp_release(const char *path, struct fuse_file_info *fi) {
  /* Just a stub.  This method is optional and can safely be left
     unimplemented */
  (void) path;
  (void) fi;
  return 0;
}
static int xmp_fsync(const char *path, int isdatasync,
                     struct fuse_file_info *fi) {
  /* Just a stub.  This method is optional and can safely be left
     unimplemented */
  (void) path;
  (void) isdatasync;
  (void) fi;
  return 0;
}

