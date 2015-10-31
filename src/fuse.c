#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#include "storage/gist.h"

static const char *dummy_file_content = "I'm an example file used until we implement a mapping for storage\n";
static const char *dummy_file_path = "/dummy";

static int getattr_callback(const char *path, struct stat *stbuf)
{
  int res = 0;

  memset(stbuf, 0, sizeof(struct stat));
  if (strcmp(path, "/") == 0) {
    stbuf->st_mode = S_IFDIR | 0755;
    stbuf->st_nlink = 2;
  } else if (strcmp(path, dummy_file_path) == 0) {
    stbuf->st_mode = S_IFREG | 0777;
    stbuf->st_nlink = 1;
    stbuf->st_size = strlen(dummy_file_content);
  } else {
    res = -ENOENT;
  }

  return res;
}

static int readdir_callback(const char *path, void *buf, fuse_fill_dir_t filler,
    off_t offset, struct fuse_file_info *fi)
{
  (void) offset;
  (void) fi;

  if (strcmp(path, "/") != 0) {
    return -ENOENT;
  }

  filler(buf, ".", NULL, 0);
  filler(buf, "..", NULL, 0);
  filler(buf, dummy_file_path + 1, NULL, 0);

  return 0;
}

static int open_callback(const char *path, struct fuse_file_info *fi)
{
  if (strcmp(path, dummy_file_path) != 0) {
    return -ENOENT;
  }

  /*if ((fi->flags & 3) != O_RDONLY) {*/
    /*return -EACCES;*/
  /*}*/

  return 0;
}

static int read_callback(const char *path, char *buf, size_t size, off_t offset,
    struct fuse_file_info *fi)
{
  size_t len;
  (void) fi;
  if(strcmp(path, dummy_file_path) != 0) {
    return -ENOENT;
  }

  len = strlen(dummy_file_content);

  if (offset < len) {
    if (offset + size > len) {
      size = len - offset;
    }
    memcpy(buf, dummy_file_content + offset, size);
  } else {
    size = 0;
  }

  return size;
}

static int write_callback(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
  char *result = storage_write(buf);
  return size;
}

static int truncate_callback(const char *path, off_t offset)
{
  return 0;
}

static struct fuse_operations webfs_fuse_ops = {
  .getattr    = getattr_callback,
  .readdir    = readdir_callback,
  .open       = open_callback,
  .read       = read_callback,
  .write      = write_callback,
  .truncate   = truncate_callback,
};

