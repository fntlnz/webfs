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

#define GIST_API_URL "https://api.github.com/gists"

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
  httpReqHeaders = curl_slist_append(httpReqHeaders,
      "cache-control: no-cache");
  httpReqHeaders = curl_slist_append(httpReqHeaders, "User-Agent: webFS");
  httpReqHeaders = curl_slist_append(httpReqHeaders,
      "Content-Type: application/json");

}

Gist::Gist(const string &accessTocken) :
    Gist(){
  withAuth=true;
  string temp("Authorization: token ");
  temp.append(accessTocken);

  httpReqHeaders = curl_slist_append(httpReqHeaders, temp.c_str());
}

pCURL Gist::getBaseRemoteRequest(const std::string &appendUrl) {
  CURL * curl = curl_easy_init();
  if (appendUrl.empty())
    curl_easy_setopt(curl, CURLOPT_URL, GIST_API_URL);
  else
    curl_easy_setopt(curl, CURLOPT_URL, ((GIST_API_URL"/")+appendUrl).c_str());
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, httpReqHeaders);
  return pCURL(curl);
}

//TODO use span instead of vector? -> is not copied so is ok vector..
std::string Gist::write(const std::vector<char> &buf) {

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
    return resp["id"].GetString();
  } else
    throw system_error(500, std::system_category(),
        "Error parsing the response");

}

std::vector<char> Gist::read(const std::string &remoteId) {

  pCURL curl = getBaseRemoteRequest(remoteId);

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

bool Gist::remove(const std::string &remoteId){
  if(!withAuth)
	  return false;

  pCURL curl = getBaseRemoteRequest(remoteId);
  curl_easy_setopt(curl.get(), CURLOPT_CUSTOMREQUEST, "DELETE");

  CURLcode respCode;
  std::string respData; //not used

  std::tie(respCode, respData) = CurlUtil::sendRequest(curl);

  CurlUtil::checkValidResponse(respCode, curl);
  long httpCode;
  curl_easy_getinfo(curl.get(), CURLINFO_RESPONSE_CODE, &httpCode);
  return httpCode==204;
}

std::string Gist::update(const std::string &remoteId,const std::vector<char> &newData){
  pCURL curl = getBaseRemoteRequest(remoteId);
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
	return resp["id"].GetString();
  } else
	throw system_error(500, std::system_category(),
		"Error parsing the response");

}

