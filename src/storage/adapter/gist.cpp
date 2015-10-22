#include "storage/adapter/gist.h"
#include <curl/curl.h>
#include <string>
#include <stdlib.h>
#include <json/json.h>
#include <cstring>

void init_string(struct string *s) {
  s->len = 0;
  s->ptr = (char *) malloc(s->len + 1);
  if (s->ptr == NULL) {
    fprintf(stderr, "malloc() failed\n");
    exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
}

namespace Storage {
namespace Adapter {
Result Gist::write() {
  CURL *curl = curl_easy_init();

  curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
  curl_easy_setopt(curl, CURLOPT_URL, "https://api.github.com/gists");

  struct curl_slist *headers = NULL;
  headers = curl_slist_append(headers, "cache-control: no-cache");
  headers = curl_slist_append(headers, "User-Agent: Webfs");
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

  curl_easy_setopt(curl,
                   CURLOPT_POSTFIELDS,
                   "{\"description\": \"the description for this gist\", \"public\": true, \"files\": { \"file1.txt\": { \"content\": \"String file contents\"}}}");

  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);

  struct string s;
  init_string(&s);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
  CURLcode ret = curl_easy_perform(curl);

  Json::Value root;
  Json::Reader reader;
  bool parsingSuccessful = reader.parse(s.ptr, root);
  const Json::Value url = root["url"];

  Result result;
  result.uri = url.asString();
  return result;
}

size_t Gist::writefunc(void *ptr, size_t size, size_t nmemb, struct string *s) {
  size_t new_len = s->len + size * nmemb;
  s->ptr = (char *) realloc(s->ptr, new_len + 1);
  if (s->ptr == NULL) {
    fprintf(stderr, "realloc() failed\n");
    exit(EXIT_FAILURE);
  }
  memcpy(s->ptr + s->len, ptr, size * nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return size * nmemb;
}
}
}
