#include <curl/curl.h>
#include <iostream>
#include <algorithm>
#include <system_error>

#include "rapidjson/document.h"

#include "storage/gist.h"
#include "storage/CurlUtil.h"

using namespace webfs::storage;
using rapidjson::Document;
using std::system_error;
using std::string;
using std::vector;

#define GIST_API_URL "https://api.github.com/gists"
#define HTTP_OK_STATUS 200

//NOTE: curl_slist_append made a copy of the string parameter

Gist::Gist(){
	httpReqHeaders = curl_slist_append(httpReqHeaders, "cache-control: no-cache");
	httpReqHeaders = curl_slist_append(httpReqHeaders, "User-Agent: webFS");
	httpReqHeaders = curl_slist_append(httpReqHeaders, "Content-Type: application/json");

}

Gist::Gist(const string &accessTocken):Gist(){
	string temp("Authorization: token ");
	temp.append(accessTocken);
	httpReqHeaders = curl_slist_append(httpReqHeaders,  accessTocken.c_str());
}


Gist::pCURL Gist::getBaseRemoteRequest(const char* url){
	pCURL curl(curl_easy_init());
	if(url == nullptr)
		curl_easy_setopt(curl.get(), CURLOPT_URL, GIST_API_URL);
	else
		curl_easy_setopt(curl.get(), CURLOPT_URL, url);
	curl_easy_setopt(curl.get(), CURLOPT_HTTPHEADER, httpReqHeaders);
	return curl;
}

void Gist::checkValidResponse(const CURLcode req,const pCURL &curl){
	if(req!=CURLE_OK)
		throw system_error(req,std::system_category(),"Connection Error");
	//else
	long httpCode = 0;
	curl_easy_getinfo (curl.get(), CURLINFO_RESPONSE_CODE, &httpCode);
	if (httpCode != HTTP_OK_STATUS)
		throw system_error(httpCode,std::system_category(),"Invalud http response code");
}

//TODO use span instead of vector? -> is not copied so is ok vector..
std::string Gist::write(const std::vector<char> &buf){

}

std::vector<char> Gist::read(const std::string &remoteId){


	std::string reqUrl(GIST_API_URL "/");
	reqUrl.append(remoteId);

	pCURL curl = getBaseRemoteRequest(reqUrl.c_str());

	std::vector<char> readData;
	CURLcode respCode;
	std::string respData;

	std::tie(respCode,respData) = CurlUtil::sendRequest(curl.get());
	checkValidResponse(respCode,curl);

	std::cout<<respData<<std::endl;
	Document respDoc;
	respDoc.Parse(respData.c_str());
	if(respDoc.HasMember("files")){
		//we have to use memberBegin because is not recognize as an array..
		const auto &fileDataValue = (respDoc["files"].MemberBegin()->value)["content"];
		const auto length = fileDataValue.GetStringLength();
		readData.resize(length);
		std::copy(fileDataValue.GetString(),
				fileDataValue.GetString()+length,
				std::begin(readData));
	}else{
		//TODO rise exception
	}

	return readData;
}

/*
bool gists::create(Node *node){

	CURL *curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(curl, CURLOPT_URL, );

	struct curl_slist *headers = NULL;

	headers = curl_slist_append(headers, "Authorization: token " TOCKEN);
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

	resp.Parse(rensponseData.c_str());
	if(resp.HasMember("id")){
		GistsRemoteInfoManager(node->getRemoteInfo()).setRemoteId(resp["id"].GetString());
		return true;
	}

	return false;
}

bool gists::download(Node *node){
	CURL *curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
	std::string getUrl("https://api.github.com/gists/");
	getUrl.append(GistsRemoteInfoManager(node->getRemoteInfo()).getRemoteId());
	curl_easy_setopt(curl, CURLOPT_URL, getUrl.c_str());

	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "cache-control: no-cache");
	headers = curl_slist_append(headers, "User-Agent: webFS");
	headers = curl_slist_append(headers, "Authorization: token " TOCKEN);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	std::string rensponseData;

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &rensponseData);

	CURLcode ret = curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	std::cout<<rensponseData<<std::endl;
	rapidjson::Document resp;
	resp.Parse(rensponseData.c_str());
	if(resp.HasMember("files")){
		std::cout<<resp["files"][node->getName()]["content"].GetString()<<std::endl;
		//node->setRemoteId(resp["id"].GetString());
		return true;
	}
	return false;
}

bool gists::remove(Node *node){
	CURL *curl = curl_easy_init();

	std::string getUrl("https://api.github.com/gists/");
	getUrl.append(GistsRemoteInfoManager(node->getRemoteInfo()).getRemoteId());

	curl_easy_setopt(curl, CURLOPT_URL, getUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");

	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "cache-control: no-cache");
	headers = curl_slist_append(headers, "User-Agent: webFS");
	headers = curl_slist_append(headers, "Authorization: token " TOCKEN);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);


	CURLcode ret = curl_easy_perform(curl);
	uint32_t httpCode;
	curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &httpCode);
	curl_easy_cleanup(curl);
	std::cout<<httpCode<<std::endl;
	return httpCode==204;
}

bool gists::update(Node *node){

	CURL *curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
	std::string getUrl("https://api.github.com/gists/");
	getUrl.append(GistsRemoteInfoManager(node->getRemoteInfo()).getRemoteId());

	curl_easy_setopt(curl, CURLOPT_URL, getUrl.c_str());

	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "cache-control: no-cache");
	headers = curl_slist_append(headers, "User-Agent: webFS");
	headers = curl_slist_append(headers, "Authorization: token " TOCKEN);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	std::string message("{\"description\": \"edit\", \"public\": true,"
			" \"files\": { \""+node->getName()+"\": { \"content\": \"Empty2\"}}}");

	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, message.c_str());

	std::string rensponseData;

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &rensponseData);

	CURLcode ret = curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	std::cout<<rensponseData<<std::endl;
	rapidjson::Document resp;

	resp.Parse(rensponseData.c_str());
	if(resp.HasMember("id")){
		return true;
	}

	return false;
}

*/

