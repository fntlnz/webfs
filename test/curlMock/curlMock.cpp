
#include "curlMock.h"
#include <cstdarg>

#include <algorithm>
#include <vector>
#include <queue>
#include <string>
#include <map>

static bool slist_free_all_is_call;
static bool easy_clean_up_is_call;
static std::vector<std::string> headerParam;
static std::map<CURLoption,std::string> reqOption;
static std::queue<std::string> response;
static size_t (*write_callback)(const char *ptr, size_t size, size_t nmemb, void *userdata);
static void* userData;
static long responseCode;

void curlMock_init(){
	slist_free_all_is_call=false;
	easy_clean_up_is_call=false;
	headerParam.clear();
	reqOption.clear();
	write_callback=nullptr;
	userData=nullptr;
	response = std::queue<std::string>();
	responseCode=0;
}

bool curl_slist_free_all_is_call(){
	return slist_free_all_is_call;
}

void curl_slist_free_all(struct curl_slist *list){
	slist_free_all_is_call=true;
}

struct curl_slist *curl_slist_append(struct curl_slist *prev, const char *data){
	headerParam.emplace_back(data);
	return prev;
}

bool curlMock_headerHasString(const std::string &text){
	return std::any_of(std::begin(headerParam),std::end(headerParam),
			[&text](const std::string &val){return val.find(text)!= std::string::npos;});
}



CURLcode curl_easy_setopt(CURL *curl, CURLoption option,...){
	va_list arg;
	va_start(arg,option);
	if(option==CURLOPT_WRITEFUNCTION)
		write_callback=va_arg(arg,size_t(*)(const char *, size_t, size_t, void *));
	else if(option==CURLOPT_WRITEDATA)
		userData = va_arg(arg,void*);
	else
		reqOption.emplace(option,va_arg(arg,char*));
	return CURLE_OK;
}

const std::string& curlMock_getOptionValue(CURLoption option){
	return reqOption[option];
}

CURLcode curl_easy_perform(CURL *curl){
	if(response.size()==0)
		return CURLE_GOT_NOTHING;

	const std::string &resp = response.front();
	if(write_callback!=nullptr){
		write_callback(resp.c_str(),sizeof(std::string::value_type),resp.size(),userData);
		response.pop();
		return CURLE_OK;
	}else{
		return CURLE_FAILED_INIT;
	}
}

void curl_easy_cleanup(CURL *curl){
	easy_clean_up_is_call=true;
}

bool curlMock_cleanup_isCall(){
	return easy_clean_up_is_call;
}

void curlMock_enqueuResponse(const std::string &text){
	response.push(text);
}

void curlMock_setResponseCode(long code){
	responseCode=code;
}

CURLcode curl_easy_getinfo (CURL *curl, CURLINFO info,...){
	va_list arg;
	va_start(arg,info);
	if(info == CURLINFO_RESPONSE_CODE){
		*va_arg(arg,long*) = responseCode;
		return CURLE_OK;
	}else
		return CURLE_FAILED_INIT;
}
