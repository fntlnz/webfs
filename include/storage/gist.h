#include <memory>
#include <curl/curl.h>
#include "storage/storage.h"
#include "storage/CurlUtil.h"

#ifndef WEBFS_STORAGE_GIST_H_
#define WEBFS_STORAGE_GIST_H_

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

  std::string write(const std::vector<char> &buf) override;

  std::vector<char> read(const std::string &remoteId) override;

  virtual ~Gist() {
    curl_slist_free_all(httpReqHeaders);
  }

private:

  /**
   * get a CURL request with the default parameters
   * @param url url to request, or the default api url
   * @return unique_ptr to a curl object
   */
  CurlUtil::pCURL getBaseRemoteRequest(const std::string &url = { });

  /**
   * append the default http header data
   */
  void setHttpRequestHeader();

  /**
   * list of http header to append to each request
   */
  struct curl_slist *httpReqHeaders;

};

}
} // namespace
#endif
