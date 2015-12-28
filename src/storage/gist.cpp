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

using std::string;
using std::vector;
using std::system_error;

#define GIST_API_URL "https://api.github.com/gists"

//NOTE: curl_slist_append made a copy of the string parameter

Gist::Gist() :
  httpReqHeaders(nullptr) {
  httpReqHeaders = curl_slist_append(httpReqHeaders,
      "cache-control: no-cache");
  httpReqHeaders = curl_slist_append(httpReqHeaders, "User-Agent: webFS");
  httpReqHeaders = curl_slist_append(httpReqHeaders,
      "Content-Type: application/json");

}

Gist::Gist(const string &accessTocken) :
    Gist() {
  string temp("Authorization: token ");
  temp.append(accessTocken);
  httpReqHeaders = curl_slist_append(httpReqHeaders, temp.c_str());
}

pCURL Gist::getBaseRemoteRequest(const std::string &url) {
  CURL * curl = curl_easy_init();
  if (url.empty())
    curl_easy_setopt(curl, CURLOPT_URL, GIST_API_URL);
  else
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, httpReqHeaders);
  return pCURL(curl);
}


//TODO use span instead of vector? -> is not copied so is ok vector..
std::string Gist::write(const std::vector<char> &buf) {

  pCURL curl = getBaseRemoteRequest();
  curl_easy_setopt(curl.get(), CURLOPT_CUSTOMREQUEST, "POST");

  std::ostringstream oss;
  oss << "{\"files\": { \"WebfsFile\": { \"content\":\"";

  std::copy(std::begin(buf), std::end(buf), std::ostream_iterator<char>(oss));

  oss << "\"}}}";
  std::string message = oss.str();

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

  std::string reqUrl(GIST_API_URL "/");
  reqUrl.append(remoteId);

  pCURL curl = getBaseRemoteRequest(reqUrl);

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
/*
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

