#ifndef TEST_CURLMOCK_CURLMOCK_H_
#define TEST_CURLMOCK_CURLMOCK_H_

#include <string>
#include <curl/curl.h>

void curlMock_init();
bool curlMock_slist_free_all_is_call();
bool curlMock_headerHasString(const std::string &text);
//todo do a perferct parma forwarding
void curlMock_enqueuResponse(const std::string &text);
//define a setOpt with 3 args
template <typename T>
CURLcode curl_easy_setopt(CURL *curl, CURLoption option, ...);
const std::string& curlMock_getOptionValue(CURLoption option);

void curlMock_setResponseCode(long code);
CURLcode curl_easy_getinfo(CURL *curl, CURLINFO info,...);


#endif /* TEST_CURLMOCK_CURLMOCK_H_ */
