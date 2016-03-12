#ifndef WEBFS_FILESYSTEM_H_
#define WEBFS_FILESYSTEM_H_

#include <string>

#include "node.h"
#include "storage/gist.h"

namespace webfs {
class Filesystem {
  public:
    Filesystem(Node *root): rootNode(root){}

    Node* createElementDirectory(const std::string &path);
    Node* createElementFile(const std::string &path);
    Node* findNode(const std::string &path);
    int writeChunk(const std::string &path, const char *buf, size_t size, off_t offset);

    storage::Storage *storage; //TODO(fntlnz): should be private
  private:
    Node *rootNode;
};
}
#endif // WEBFS_FILESYSTEM_H_
