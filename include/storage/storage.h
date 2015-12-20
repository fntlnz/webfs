#include <string>

#ifndef WEBFS_STORAGE_STORAGE_H_
#define WEBFS_STORAGE_STORAGE_H_
namespace webfs {
namespace storage {
class Storage {
  public:
    virtual std::string write(const std::string &buf) = 0;
};
}
}
#endif
