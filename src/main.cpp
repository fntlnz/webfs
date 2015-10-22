#include <iostream>
#include "fuse.cpp"

using namespace std;

int main(int argc, char *argv[]) {
    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}