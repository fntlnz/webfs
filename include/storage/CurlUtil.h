#ifndef INCLUDE_STORAGE_CURLUTIL_H_
#define INCLUDE_STORAGE_CURLUTIL_H_

#include <string>
#include <tuple>
#include <curl/curl.h>

namespace webfs{
namespace storage{
  class CurlUtil{
    public:

	  static std::tuple<CURLcode,std::string> sendRequest(CURL *curl){
		  std::string rensponseData;

		  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlUtil::writefunc);
		  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &rensponseData);

		  CURLcode ret = curl_easy_perform(curl);

		  return std::make_tuple(ret,rensponseData);

	  }
/*
	  using reqCallback_t = std::function<void(const CURLcode resp, const std::string&)>;

	  static void sendRequest(CURL *curl,reqCallback_t onRensponse){
				onRensponse(ret,rensponseData);
				  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlUtil::writefunc);
					  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &rensponseData);

					  CURLcode ret = curl_easy_perform(curl);
					  curl_easy_cleanup(curl);

	  }
	  */


    private:
	  static size_t writefunc(void *ptr, size_t size, size_t nmemb, std::string *s){
	    s->append((const char*)ptr,size*nmemb);
	    return size * nmemb;
	  }
  };

}} //namespace webfs::storage




#endif /* INCLUDE_STORAGE_CURLUTIL_H_ */
