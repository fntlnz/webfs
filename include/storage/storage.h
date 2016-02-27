#ifndef WEBFS_STORAGE_STORAGE_H_
#define WEBFS_STORAGE_STORAGE_H_

#include <string>
#include <vector>
#include <memory>

namespace webfs {
namespace storage {
class Storage {
  public:

	class StorageId{
		public:
			virtual ~StorageId(){};

		//protected:
			virtual std::string getRemoteWriteUrl()const =0;
			virtual std::string getRemoteReadUrl()const =0 ;
			virtual std::string getRemoteEditUrl()const =0;
			virtual std::string getRemoteRemoveUrl()const =0;

		friend class Storage;

	};

	using pStorageId = std::unique_ptr<StorageId>;

  /**
   * upload the data in a new gist
   * @param buf data to upload
   * @return remote identifier
   */
    virtual pStorageId write(const std::vector<char> &buf) = 0;
    /**
   * read the data from a remote gist
   * @param remoteId remote identifier
   * @return remote identifier
   */
    virtual std::vector<char> read(const pStorageId &remoteId)=0;

    virtual bool remove(const pStorageId &remoteId)=0;

    virtual pStorageId update(const pStorageId &remoteId,
    		  const std::vector<char> &newData)=0;


    virtual ~Storage(){};
};
}
}
#endif
