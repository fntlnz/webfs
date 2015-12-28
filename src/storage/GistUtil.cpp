#include "storage/GistUtil.h"

using namespace webfs::storage;
#define GITHUB_AUTH_URL "https://api.github.com/authorizations"

std::string GistUtil::getAuthToken(const std::string &user,const std::string &pass,
		const std::string &tokenNote,const std::string &userAgent){
  using webfs::storage::CurlUtil;
  using pCURL=CurlUtil::pCURL;

  pCURL curl(curl_easy_init());
  struct curl_slist *header=nullptr;
  header = curl_slist_append(header,  "Content-Type: application/json");
  header = curl_slist_append(header, ("User-Agent: "+userAgent).c_str());
  curl_easy_setopt(curl.get(), CURLOPT_URL,GITHUB_AUTH_URL);
  curl_easy_setopt(curl.get(), CURLOPT_HTTPHEADER, header);
  curl_easy_setopt(curl.get(), CURLOPT_CUSTOMREQUEST, "POST");
  curl_easy_setopt(curl.get(), CURLOPT_HTTPAUTH,CURLAUTH_BASIC);
  curl_easy_setopt(curl.get(), CURLOPT_USERNAME, user.c_str());
  curl_easy_setopt(curl.get(), CURLOPT_PASSWORD, pass.c_str());

  std::string msg ="{\"scopes\":[\"gist\"],\"note\":\""+tokenNote+"\"}";
  curl_easy_setopt(curl.get(), CURLOPT_POSTFIELDS,msg.c_str());

  std::string respData;
  CURLcode respCode;
  std::tie(respCode, respData) = CurlUtil::sendRequest(curl);
  //free the header structure before have the possibility to thow an exception
  curl_slist_free_all(header);

  CurlUtil::checkValidResponse(respCode, curl, 201);

  rapidjson::Document respDoc;
  respDoc.Parse(respData.c_str());
  if (respDoc.HasMember("token")) {
	return respDoc["token"].GetString();
  }else{
	throw std::system_error(500, std::system_category(),
		"Error parsing the response");
}//getAuthToken

}
