#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <libgen.h>

#include "storage/gist.h"
#include "tree.h"

static const char *dummy_file_content = "I'm an example file used until we implement a mapping for storage\n";
static const char *dummy_file_path = "/dummy";

static node_t *root_node;

static node_t *get_root_node()
{
  if (root_node != NULL) {
    return root_node;
  }
  root_node = create_node();
  root_node->type = TYPE_FOLDER;
  root_node->name = "";
  root_node->num_children = 0;
  root_node->children = malloc(sizeof(node_t));
  return root_node;
}

static void add_child(node_t *parent, const char *file_name)
{
  parent->num_children++;
  parent->children = realloc(parent->children, sizeof(node_t) * parent->num_children);
  node_t *curnode = create_node();
  curnode->type = TYPE_FILE;
  curnode->num_children = 0;
  curnode->parent = parent;
  curnode->name = strdup(file_name);
  parent->children[parent->num_children - 1] = curnode;
}

static node_t *find_node(const char *path, node_t *root)
{
  int i;
  int count = (int) root->num_children;
  for (i = 0; i < count; i++) {
    node_t *child = root->children[i];
    // TODO: its just a one level stub for now I have to allow search in subpaths
    char *npath = strdup(path);
    if (strcmp(child->name, basename(npath)) == 0) {
      return child;
    }
  }
  return NULL;
}

static int getattr_callback(const char *path, struct stat *stbuf)
{
  int res = 0;
  memset(stbuf, 0, sizeof(struct stat));

  if (strcmp(path, "/") == 0) {
    stbuf->st_mode = S_IFDIR | 0755;
    stbuf->st_nlink = 2;
    return res;
  }

  if (strcmp(path, dummy_file_path) == 0) {
    stbuf->st_mode = S_IFREG | 0777;
    stbuf->st_nlink = 1;
    stbuf->st_size = strlen(dummy_file_content);
    return res;
  }

  node_t *node = find_node(path, get_root_node());

  if (node != NULL) {
    stbuf->st_mode = S_IFREG | 0777;
    stbuf->st_nlink = 1;
    return res;
  }

  return -ENOENT;
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
  return 0;
}

static int read_callback(const char *path, char *buf, size_t size, off_t offset,
    struct fuse_file_info *fi)
{
  size_t len = 0;
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
  }

  return size;
}

static int write_callback(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
  node_t *node = find_node(path, get_root_node());
  /*char *result = storage_write(buf);*/
  return size;
}

static int truncate_callback(const char *path, off_t offset)
{
  return 0;
}

static int mknod_callback(const char *path, mode_t mode, dev_t dev)
{
  return 0;
}

static int create_callback (const char *path, mode_t mode, struct fuse_file_info *fi)
{
  node_t *root = get_root_node();
  add_child(root, basename(strdup(path)));
  return 0;
}

static struct fuse_operations webfs_fuse_ops = {
  .getattr    = getattr_callback,
  .readdir    = readdir_callback,
  .open       = open_callback,
  .read       = read_callback,
  .write      = write_callback,
  .truncate   = truncate_callback,
  .mknod      = mknod_callback,
  .create     = create_callback,
};

