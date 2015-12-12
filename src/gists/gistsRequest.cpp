/*
 * gistsRequest.cpp
 *
 *  Created on: Dec 12, 2015
 *      Author: wise
 */

#include <curl/curl.h>
#include <iostream>

#include "rapidjson/document.h"
#include "node.h"
#include "gists/gistsRequest.h"

using namespace webfs;

static size_t writefunc(void *ptr, size_t size, size_t nmemb, std::string *s){
  s->resize(s->size()+size*nmemb);
  s->append((const char*)ptr,size*nmemb);

  return size * nmemb;
}

bool gists::create(Node *node){

	CURL *curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(curl, CURLOPT_URL, "https://api.github.com/gists");

	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "cache-control: no-cache");
	headers = curl_slist_append(headers, "User-Agent: webFS");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	std::string message("{\"description\": \"\", \"public\": true,"
			" \"files\": { \""+node->getName()+"\": { \"content\": \"Empty\"}}}");

	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, message.c_str());

	std::string rensponseData;

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &rensponseData);

	CURLcode ret = curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	std::cout<<rensponseData<<std::endl;
	rapidjson::Document resp;
	rensponseData+='\0';
	resp.Parse(rensponseData.c_str());
	assert(resp.IsObject());
	//if(resp.HasMember("id")){
		//std::cout<<"ok"<<std::endl;
		node->setRemoteId(resp["id"].GetString());
		return true;
	//}

	return false;


}
