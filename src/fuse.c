#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <libgen.h>

#include "storage/gist.h"
#include "tree.h"

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
  root_node->children = malloc(sizeof(node_t*));
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

static char **split_path(const char *src)
{
  const char *separator = "/";
  char *tok = strtok(strdup(src), separator);

  if(tok == NULL) {
    return NULL;
  }

  char **nodes = malloc(sizeof(char *));
  nodes[0] = tok;
  int nodes_num = 1;
  while((tok = strtok(NULL, separator)))  {
    nodes = realloc(nodes, sizeof(nodes) * sizeof(char));
    nodes[nodes_num] = tok;
    nodes_num++;
  }
  return nodes;
}

static node_t *find_node(const char *path, node_t *root, int level)
{
  int i;
  int count = (int) root->num_children;
  char **path_arr = split_path(path);

  for (i = 0; i < count; i++) {
    node_t *node = root->children[i];
    if (strcmp(node->name, path_arr[level]) == 0) {
      if (sizeof(path_arr) / sizeof(char *) == level + 1) {
        return node;
      }
      return find_node(path, node, level+1);
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

  node_t *node = find_node(path, get_root_node(), 0);

  if (node != NULL) {
    stbuf->st_mode = S_IFREG | 0777;
    stbuf->st_nlink = 1;
    stbuf->st_size = node->size;
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

  int i;
  node_t *root = get_root_node();
  int count = (int) root->num_children;
  for (i = 0; i < count; i++) {
    filler(buf, root->children[i]->name, NULL, 0);
  }

  return 0;
}

static int open_callback(const char *path, struct fuse_file_info *fi)
{
  return 0;
}

static int read_callback(const char *path, char *buf, size_t size, off_t offset,
    struct fuse_file_info *fi)
{
  size_t len;
  (void) fi;

  node_t *node = find_node(path, get_root_node(), 0);

  if (node == NULL) {
    return -ENOENT;
  }

  len = node->size;

  if (offset < len) {
    if (offset + size > len) {
      size = len - offset;
    }

    if (!sizeof(node->location)/sizeof(node->location[0]) > 0) {
      return 0;
    }

    const char *content = storage_read(node->location[0]->location);
    memcpy(buf, content + offset, size);
    return size;
  }

  return 0;
}

static int write_callback(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
  node_t *node = find_node(path, get_root_node(), 0);
  char *result = storage_write(buf);
  if (result == NULL) {
    return 0;
  }
  node->location = malloc(sizeof(node_location_t *));
  node_location_t *location = malloc(sizeof(char *));
  location->location = result;
  node->location[0] = location;
  node->size = strlen(buf);
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

