#ifndef WEBFS_FILESYSTEM_H_
#define WEBFS_FILESYSTEM_H_

#include <string>

#include "node.h"
#include "storage/gist.h"

namespace webfs {
class Filesystem {
  public:
    Filesystem(std::shared_ptr<Node> &root): rootNode(root){}

    std::weak_ptr<Node> createElementDirectory(const std::string &path);
    std::weak_ptr<Node> createElementFile(const std::string &path);
    std::weak_ptr<Node> findNode(const std::string &path);
    int writeChunk(const std::string &path, const char *buf, size_t size, off_t offset);

    storage::Storage *storage; //TODO(fntlnz): should be private
  private:
    std::shared_ptr<Node> rootNode;
};
}
#endif // WEBFS_FILESYSTEM_H_
