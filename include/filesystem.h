#include <string>
#include "node.h"
#ifndef WEBFS_FILESYSTEM_H_
#define WEBFS_FILESYSTEM_H_
namespace webfs {
class Filesystem {
  public:
    Filesystem(Node *root): rootNode(root){}
    Node *createDirectory(const std::string &path);
    Node *createFile(const std::string &path);
    Node *findNode(const std::string &path);
  private:
    Node *rootNode;
};
}
#endif // WEBFS_FILESYSTEM_H_
