#include "node.h"
#include "filesystem.h"
#include "node_serializer.h"

#include <iostream>

#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <cstring>
#ifdef HAVE_SETXATTR
#include <sys/xattr.h>
#endif

webfs::Filesystem *filesystem;
static struct fuse_operations webfs_fuse_ops;

static int getattr_callback(const char *path, struct stat *stbuf) {
  memset(stbuf, 0, sizeof(struct stat));
  std::string p(path);

  if (p == "/") {
    p = "";
  }

  auto node = filesystem->findNode(p).lock();

  if (node == nullptr) {
    return -ENOENT;
  }

  if (node->getType() == webfs::Node::Type::LEAF) {
    stbuf->st_mode = S_IFREG | 0777;
    stbuf->st_nlink = 1;
    stbuf->st_size = 10; // TODO: add size to node
    return 0;
  }

  if (node->getType() == webfs::Node::Type::BRANCH) {
    stbuf->st_mode = S_IFDIR | 0755;
    stbuf->st_nlink = 2;
    return 0;
  }

  return 0;
}

static int readdir_callback(const char *path, void *buf, fuse_fill_dir_t filler,
    off_t offset, struct fuse_file_info *fi) {
  (void) offset;
  (void) fi;
  std::string p = std::string(path, strlen(path));

  filler(buf, ".", NULL, 0);
  filler(buf, "..", NULL, 0);

  const auto directory = filesystem->findNode(p).lock();
  for(const auto n : directory->getChildren()) {
    filler(buf, n.lock()->getName().c_str(), NULL, 0);
  }

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

  std::string p = std::string(path);
  return filesystem->writeChunk(p, buf, size, offset);
}

static int truncate_callback(const char *path, off_t offset) {
  return 0;
}

static int mknod_callback(const char *path, mode_t mode, dev_t dev) {
  return 0;
}

static int create_callback(const char *path, mode_t mode, struct fuse_file_info *fi) {
  const std::string p(path);
  filesystem->createElementFile(p);
  return 0;
}

static int utime_callback(const char *path, struct utimbuf *buf) {
  // TODO: handle utime in a proper way.
  return 0;
}

static int mkdir_callback(const char *path, mode_t mode) {
  const std::string p(path);
  filesystem->createElementDirectory(p);
  return 0;
}

extern "C" {

static fuse_operations init_fuse_operations() {
  webfs_fuse_ops.getattr = getattr_callback;
  webfs_fuse_ops.readlink = NULL;
  webfs_fuse_ops.getdir = NULL;
  webfs_fuse_ops.mknod = mknod_callback;
  webfs_fuse_ops.mkdir = mkdir_callback;
  webfs_fuse_ops.unlink = NULL;
  webfs_fuse_ops.rmdir = NULL;
  webfs_fuse_ops.symlink = NULL;
  webfs_fuse_ops.rename = NULL;
  webfs_fuse_ops.link = NULL;
  webfs_fuse_ops.chmod = NULL;
  webfs_fuse_ops.chown = NULL;
  webfs_fuse_ops.truncate = truncate_callback;
  webfs_fuse_ops.utime = utime_callback;
  webfs_fuse_ops.open = open_callback;
  webfs_fuse_ops.read = read_callback;
  webfs_fuse_ops.write = write_callback;
  webfs_fuse_ops.flush = NULL;
  webfs_fuse_ops.create = create_callback;
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

int main(int argc, char *argv[]) {

  std::string json = "{\"isLeaf\":false,\"name\":\"\",\"children\":[{\"isLeaf\":false,\"name\":\"I AM A DEFAULT FOLDER\",\"children\":[]}]}";
  rapidjson::Document readNode;
  readNode.Parse(json.c_str());

  auto root = webfs::NodeSerializer::unserialize(readNode);

  filesystem = new webfs::Filesystem(root);
  filesystem->storage = new webfs::storage::Gist();

  fuse_operations ops = init_fuse_operations();
  return fuse_main(argc, argv, &ops, NULL);
}

