#include <memory>
#include <curl/curl.h>
#include "storage.h"

#ifndef WEBFS_STORAGE_GIST_H_
#define WEBFS_STORAGE_GIST_H_

namespace webfs {
namespace storage {

class Gist : public Storage {
  public:
	Gist();
	Gist(const std::string &accessTocken);

    std::string write(const std::vector<char> &buf) override;
    std::vector<char> read(const std::string &remoteId) override;

    virtual ~Gist(){
    	curl_slist_free_all(httpReqHeaders);
    }

  private:
    struct pCURLDeleter {
      void operator()(CURL * c) { curl_easy_cleanup(c); }
    };

    using pCURL = std::unique_ptr<CURL,pCURLDeleter>;

    pCURL getBaseRemoteRequest(const char* url=nullptr);
    void setHttpRequestHeader();
    void checkValidResponse(const CURLcode req,const pCURL &curl);
    struct curl_slist *httpReqHeaders;

};
}
}
#endif
