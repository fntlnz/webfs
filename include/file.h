#include <vector>
#include <string>
#include <cstdlib>
#ifndef WEBFS_FILE_H_
#define WEBFS_FILE_H_
namespace webfs {

class FileChunk {
  public:
    size_t size;
    off_t offset;
    std::string identifier;
};

class File {
  public:
    void addChunk(FileChunk *c) {
      chunks.push_back(c);
    }

    bool empty() {
      return chunks.empty();
    }

  private:
    std::vector<FileChunk *> chunks;
};

}
#endif // WEBFS_FILE_H_
