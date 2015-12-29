#ifndef INCLUDE_STORAGE_CURLUTIL_H_
#define INCLUDE_STORAGE_CURLUTIL_H_
#include <memory>
#include <string>
#include <tuple>
#include <system_error>
#include <curl/curl.h>

namespace webfs {
namespace storage {

/**
 * utility class for send a curl request and read the response
 */
class CurlUtil {
	/**
	 * object that will free the data allocated by a curl request
	 */
	struct pCURLDeleter {
		void operator()(CURL * c) {
			curl_easy_cleanup(c);
		}
	};

public:

	/**
	 * utility type that store an unique_ptr of type CURL
	 */
	using pCURL = std::unique_ptr<CURL,pCURLDeleter>;

	/**
	 * made a curl request and wait the response
	 * @param curlptr request to do
	 * @return status of the request and the returned data
	 */
	static std::tuple<CURLcode, std::string> sendRequest(const pCURL &curlptr) {

		std::string rensponseData;
		CURL *curl = const_cast<CURL*>(curlptr.get());

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlUtil::writefunc);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &rensponseData);

		CURLcode ret = curl_easy_perform(curl);

		return std::make_tuple(ret, std::move(rensponseData));

	} //sendRequest


  /**
   * check it the request was done correctly
   * @param req curl return code
   * @param curl pointer to the call to check
   * @param validHttpRensponse required http response status
   * @throw system_error if some error happen
   */
  static void checkValidResponse(const CURLcode req, const CurlUtil::pCURL &curl,
	  const long validHttpRensponse){
	if (req != CURLE_OK)
	  throw std::system_error(req, std::system_category(), "Connection Error");
	//else
	long httpCode = 0;
	curl_easy_getinfo(curl.get(), CURLINFO_RESPONSE_CODE, &httpCode);
	if (httpCode != validHttpRensponse)
	  throw std::system_error(httpCode, std::system_category(),
		"Invalud http response code");
  }

private:
	/**
	 * curl call back, just store the data inside a string
	 */
	static size_t writefunc(void *ptr, size_t size, size_t nmemb,
			std::string *s) {
		s->append((const char*) ptr, size * nmemb);
		return size * nmemb;
	}

};

}
} //namespace webfs::storage

#endif /* INCLUDE_STORAGE_CURLUTIL_H_ */
