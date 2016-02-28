#include <curl/curl.h>
#include <iostream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <system_error>

#include "rapidjson/document.h"

#include "storage/gist.h"

using namespace webfs::storage;
using pCURL=CurlUtil::pCURL;
using rapidjson::Document;

using std::system_error;
using std::string;
using std::vector;
using pStorageId = Storage::pStorageId;

#define GIST_API_URL "https://api.github.com/gists"

class StorageIdGist : public Storage::StorageId{

	private:
		std::string mId;

	public:
		StorageIdGist(const std::string &id):mId(id){}

	protected:
		std::string getRemoteWriteUrl() const override{
			return ((GIST_API_URL"/")+mId);
		}
		std::string getRemoteReadUrl() const override{
			return ((GIST_API_URL"/")+mId);
		}
		std::string getRemoteEditUrl() const override{
			return ((GIST_API_URL"/")+mId);
		};
		std::string getRemoteRemoveUrl() const override{
			return ((GIST_API_URL"/")+mId);
		};

		friend class Gist;

	};

//NOTE: curl_slist_append made a copy of the string parameter

static std::string uploadFileRequest(const std::string &fileName,
		const std::vector<char> &content){
	std::ostringstream oss;
	oss << "{\"files\": { \""<<fileName<<"\": { \"content\":\"";

	std::copy(std::begin(content), std::end(content), std::ostream_iterator<char>(oss));

	oss << "\"}}}";
	return oss.str();
}

Gist::Gist() :
  httpReqHeaders(nullptr),withAuth(false) {
  curl_global_init(CURL_GLOBAL_NOTHING);
  httpReqHeaders = curl_slist_append(httpReqHeaders,
      "cache-control: no-cache");
  httpReqHeaders = curl_slist_append(httpReqHeaders, "User-Agent: webFS");
  httpReqHeaders = curl_slist_append(httpReqHeaders,
      "Content-Type: application/json");

}

Gist::Gist(const string &accessToken) :
    Gist(){
  withAuth=true;
  string temp("Authorization: token ");
  temp.append(accessToken);

  httpReqHeaders = curl_slist_append(httpReqHeaders, temp.c_str());
}

pCURL Gist::getBaseRemoteRequest(const std::string &url) {
  CURL *curl = curl_easy_init();
  if (url.empty())
    curl_easy_setopt(curl, CURLOPT_URL, GIST_API_URL);
  else
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, httpReqHeaders);
  return pCURL(curl);
}

//TODO use span instead of vector? -> is not copied so is ok vector..
pStorageId Gist::write(const std::vector<char> &buf) {

  pCURL curl = getBaseRemoteRequest();

  curl_easy_setopt(curl.get(), CURLOPT_CUSTOMREQUEST, "POST");

  std::string message = uploadFileRequest("WebFs",buf);

  curl_easy_setopt(curl.get(), CURLOPT_POSTFIELDS, message.c_str());

  std::string respData;
  CURLcode respCode;
  std::tie(respCode, respData) = CurlUtil::sendRequest(curl);

  CurlUtil::checkValidResponse(respCode, curl, 201);

  rapidjson::Document resp;

  resp.Parse(respData.c_str());
  if (resp.HasMember("id")) {
	  return std::make_unique<StorageIdGist>(resp["id"].GetString());
  } else
    throw system_error(500, std::system_category(),
        "Error parsing the response");

}

std::vector<char> Gist::read(const pStorageId &remoteId) {

  pCURL curl = getBaseRemoteRequest(remoteId->getRemoteReadUrl());

  std::vector<char> readData;
  CURLcode respCode;
  std::string respData;

  std::tie(respCode, respData) = CurlUtil::sendRequest(curl);

  CurlUtil::checkValidResponse(respCode, curl, 200);

  Document respDoc;
  respDoc.Parse(respData.c_str());
  if (respDoc.HasMember("files")) {
    //we have to use memberBegin because is not recognize as an array..
    const auto &fileDataValue =
        (respDoc["files"].MemberBegin()->value)["content"];
    const auto length = fileDataValue.GetStringLength();
    readData.resize(length);
    std::copy(fileDataValue.GetString(), fileDataValue.GetString() + length,
        std::begin(readData));
  } else {
    throw system_error(500, std::system_category(),
        "Error parsing the response");
  }

  return readData;
}

bool Gist::remove(const pStorageId &remoteId){
  if(!withAuth)
	  return false;

  pCURL curl = getBaseRemoteRequest(remoteId->getRemoteRemoveUrl());
  curl_easy_setopt(curl.get(), CURLOPT_CUSTOMREQUEST, "DELETE");

  CURLcode respCode;
  std::string respData; //not used

  std::tie(respCode, respData) = CurlUtil::sendRequest(curl);

  CurlUtil::checkValidResponse(respCode, curl);
  long httpCode;
  curl_easy_getinfo(curl.get(), CURLINFO_RESPONSE_CODE, &httpCode);
  return httpCode==204;
}

pStorageId Gist::update(const pStorageId &remoteId,const std::vector<char> &newData){
  pCURL curl = getBaseRemoteRequest(remoteId->getRemoteEditUrl());
  curl_easy_setopt(curl.get(), CURLOPT_CUSTOMREQUEST, "PATCH");

  const std::string message = uploadFileRequest("WebFs",newData);
  curl_easy_setopt(curl.get(), CURLOPT_POSTFIELDS, message.c_str());

  std::string respData;
  CURLcode respCode;
  std::tie(respCode, respData) = CurlUtil::sendRequest(curl);

  CurlUtil::checkValidResponse(respCode, curl, 200);

  rapidjson::Document resp;
  resp.Parse(respData.c_str());
  if (resp.HasMember("id")) {
	  //TODO check if ti change
	  return std::make_unique<StorageIdGist>(resp["id"].GetString());
  } else
	throw system_error(500, std::system_category(),
		"Error parsing the response");

}

