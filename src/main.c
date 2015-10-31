#include "fuse.c"

int main(int argc, char *argv[]) {
  umask(0);
  struct fuse_operations xmp_oper;
  xmp_oper.getattr        = xmp_getattr;
  xmp_oper.access         = xmp_access;
  xmp_oper.readlink       = xmp_readlink;
  xmp_oper.readdir        = xmp_readdir;
  xmp_oper.mknod          = xmp_mknod;
  xmp_oper.mkdir          = xmp_mkdir;
  xmp_oper.symlink        = xmp_symlink;
  xmp_oper.unlink         = xmp_unlink;
  xmp_oper.rmdir          = xmp_rmdir;
  xmp_oper.rename         = xmp_rename;
  xmp_oper.link           = xmp_link;
  xmp_oper.chmod          = xmp_chmod;
  xmp_oper.chown          = xmp_chown;
  xmp_oper.truncate       = xmp_truncate;
  xmp_oper.open           = xmp_open;
  xmp_oper.read           = xmp_read;
  xmp_oper.write          = xmp_write;
  xmp_oper.statfs         = xmp_statfs;
  xmp_oper.release        = xmp_release;
  xmp_oper.fsync          = xmp_fsync;
  return fuse_main(argc, argv, &xmp_oper, NULL);
}
