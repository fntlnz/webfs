#include "fuse_main.h"
#include "node.h"
#include <iostream>

extern "C" {

#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#ifdef HAVE_SETXATTR
#include <sys/xattr.h>
#endif

static int getattr_callback(const char *path, struct stat *stbuf) {
  return -ENOENT;
}

static int readdir_callback(const char *path, void *buf, fuse_fill_dir_t filler,
                            off_t offset, struct fuse_file_info *fi) {
  return 0;
}

static int open_callback(const char *path, struct fuse_file_info *fi) {
  return 0;
}

static int read_callback(const char *path, char *buf, size_t size, off_t offset,
                         struct fuse_file_info *fi) {
  return 0;
}

static int write_callback(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
  return 0;
}

static int truncate_callback(const char *path, off_t offset) {
  return 0;
}

static int mknod_callback(const char *path, mode_t mode, dev_t dev) {
  return 0;
}

static int create_callback(const char *path, mode_t mode, struct fuse_file_info *fi) {
  return 0;
}

static fuse_operations get_ops() {
  struct fuse_operations webfs_fuse_ops;
  webfs_fuse_ops.getattr = getattr_callback;
  webfs_fuse_ops.readlink = NULL;
  webfs_fuse_ops.getdir = NULL;
  webfs_fuse_ops.mknod = mknod_callback;
  webfs_fuse_ops.mkdir = NULL;
  webfs_fuse_ops.unlink = NULL;
  webfs_fuse_ops.rmdir = NULL;
  webfs_fuse_ops.symlink = NULL;
  webfs_fuse_ops.rename = NULL;
  webfs_fuse_ops.link = NULL;
  webfs_fuse_ops.chmod = NULL;
  webfs_fuse_ops.chown = NULL;
  webfs_fuse_ops.truncate = truncate_callback;
  webfs_fuse_ops.utime = NULL;
  webfs_fuse_ops.open = open_callback;
  webfs_fuse_ops.read = read_callback;
  webfs_fuse_ops.write = write_callback;
  webfs_fuse_ops.statfs = NULL;
  webfs_fuse_ops.flush = NULL;
  webfs_fuse_ops.release = NULL;
  webfs_fuse_ops.fsync = NULL;
  webfs_fuse_ops.setxattr = NULL;
  webfs_fuse_ops.getxattr = NULL;
  webfs_fuse_ops.listxattr = NULL;
  webfs_fuse_ops.removexattr = NULL;
  webfs_fuse_ops.opendir = NULL;
  webfs_fuse_ops.readdir = readdir_callback;
  webfs_fuse_ops.releasedir = NULL;
  webfs_fuse_ops.fsyncdir = NULL;
  webfs_fuse_ops.init = NULL;
  return webfs_fuse_ops;
}

} // extern "C"

int mount(int argc, char *argv[]) {
  fuse_operations ops = get_ops();
  return fuse_main(argc, argv, &ops, NULL);
}

