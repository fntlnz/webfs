#ifndef TEST_CURLMOCK_CURLMOCK_H_
#define TEST_CURLMOCK_CURLMOCK_H_

#include <string>
#include <curl/curl.h>

/**
 * this file implments some method for have fake call to the curl library
 */

/**
 * initialize the mock library
 */
void curlMock_init();

/**
 * true if the use call the method curl_slist_free_all
 */
bool curlMock_slist_free_all_isCall();

/**
 * true if one header string contains the parameter
 * @param text text to search
 * @return true if the use set an header that contains that text
 */
bool curlMock_headerHasString(const std::string &text);

//todo do a perferct parma forwarding
/**
 * set the text to notify to the user when we receive a request response,
 * this string will be enqueue in a fifo queue, so you can set multiple response
 * @param text string to return to the user
 */
void curlMock_enqueuResponse(const std::string &text);

/**
 * return the string set as an call option
 * @param option option name
 * @return string link to that option
 */
const std::string& curlMock_getOptionValue(CURLoption option);

/**
 * set the http response code to return after an http request
 * @param code code to return
 */
void curlMock_setResponseCode(long code);

/**
 *
 * @return true if the use call curl_easy_cleanup()
 */
bool curlMock_cleanup_isCall();


CURLcode curl_easy_setopt(CURL *curl, CURLoption option, ...);
CURLcode curl_easy_getinfo(CURL *curl, CURLINFO info,...);

#endif /* TEST_CURLMOCK_CURLMOCK_H_ */
