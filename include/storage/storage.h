#include <string>
#include <vector>

#ifndef WEBFS_STORAGE_STORAGE_H_
#define WEBFS_STORAGE_STORAGE_H_
namespace webfs {
namespace storage {
class Storage {
  public:
  /**
   * upload the data in a new gist
   * @param buf data to upload
   * @return remote identifier
   */
    virtual std::string write(const std::vector<char> &buf) = 0;
    /**
   * read the data from a remote gist
   * @param remoteId remote identifier
   * @return remote identifier
   */
    virtual std::vector<char> read(const std::string &remoteId)=0;
    virtual ~Storage(){};
};
}
}
#endif
