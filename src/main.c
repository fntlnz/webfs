#include "fuse.c"

int main(int argc, char *argv[])
{
  return fuse_main(argc, argv, &webfs_fuse_ops, NULL);
}
