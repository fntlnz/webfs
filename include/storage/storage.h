#include <string>
#include <vector>

#ifndef WEBFS_STORAGE_STORAGE_H_
#define WEBFS_STORAGE_STORAGE_H_
namespace webfs {
namespace storage {
class Storage {
  public:
    virtual std::string write(const std::vector<char> &buf) = 0;
    virtual std::vector<char> read(const std::string &remoteId)=0;
    virtual ~Storage(){};
};
}
}
#endif
