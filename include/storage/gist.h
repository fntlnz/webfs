#ifndef WEBFS_STORAGE_GIST_H_
#define WEBFS_STORAGE_GIST_H_


#include <memory>
#include <curl/curl.h>
#include "storage/storage.h"
#include "storage/CurlUtil.h"


namespace webfs {
namespace storage {

/**
 * upload/download data from a gist service,
 * all the request will have:
 *  cache-control: no-cache
 *  User-Agent: webFS
 *  Content-Type: application/json
 */
class Gist: public Storage {

public:
  /**
   * read/write gist in anonymous way
   */
  Gist();

  /**
   * read/write gist of an user
   * @param accessTocken use authentication token
   */
  Gist(const std::string &accessTocken);

  Storage::pStorageId write(const std::vector<char> &buf) override;

  std::vector<char> read(const Storage::pStorageId &remoteId) override;

  /**
   * if the gist belong to a user can be deleted
   * @param remoteId gist to remove
   * @return true if the operation can be done, false otherwise
   */
  bool remove(const Storage::pStorageId &remoteId) override;
  Storage::pStorageId update(const Storage::pStorageId &remoteId,
		  const std::vector<char> &newData) override;

  virtual ~Gist() {
    curl_slist_free_all(httpReqHeaders);
  }

private:

  /**
   * get a CURL request with the default parameters
   * @param appendUrl string that will be append to the base api address,
   * 	the / is included in the base api address
   * @return unique_ptr to a curl object
   */
  CurlUtil::pCURL getBaseRemoteRequest(const std::string &appendUrl = { });

  /**
   * append the default http header data
   */
  void setHttpRequestHeader();

  /**
   * list of http header to append to each request
   */
  struct curl_slist *httpReqHeaders;
  bool withAuth;

};

}
} // namespace
#endif
