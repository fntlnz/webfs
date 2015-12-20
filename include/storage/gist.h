#include "storage.h"
#include <iostream>
#ifndef WEBFS_STORAGE_GIST_H_
#define WEBFS_STORAGE_GIST_H_
namespace webfs {
namespace storage {
class Gist : public Storage {
  public:
    std::string write(const std::string &buf) {
      std::cout << "BUF: " << buf << std::endl;
      return "somereference"; //TODO: return the right reference from gist
    }
};
}
}
#endif
