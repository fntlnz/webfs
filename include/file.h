#ifndef WEBFS_FILE_H_
#define WEBFS_FILE_H_

#include <vector>
#include <string>
#include <cstdlib>

#include "storage/storage.h"

namespace webfs {
using pStorageId = storage::Storage::pStorageId;

class FileChunk {
  public:
    size_t size;
    off_t offset;
    pStorageId identifier;
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
