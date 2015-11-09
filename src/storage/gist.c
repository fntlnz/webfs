#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <stdlib.h>

#include "json.h"
#include "storage/gist.h"
#include "base64.h"

struct string {
  char *ptr;
  size_t len;
};

static json_value *get_json_object_value_at_key(json_value *json, const char *key)
{
  if (json == NULL) {
    return NULL;
  }
  if (json->type != json_object) {
    return NULL;
  }

  int i;
  for (i = 0; i < json->u.object.length; i++) {
    json_object_entry entry = json->u.object.values[i];
    if (strcmp(entry.name, key) == 0) {
      return entry.value;
    }
  }
  return NULL;
}

static void init_string(struct string *s)
{
  s->len = 0;
  s->ptr = (char *) malloc(s->len + 1);
  if (s->ptr == NULL) {
    fprintf(stderr, "malloc() failed\n");
    exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
}

static size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
  size_t new_len = s->len + size * nmemb;
  s->ptr = (char *) realloc(s->ptr, new_len + 1);
  if (s->ptr == NULL) {
    fprintf(stderr, "realloc() failed\n");
    return 0;
  }
  memcpy(s->ptr + s->len, ptr, size * nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return size * nmemb;
}

char *storage_write(const char *buf)
{
  CURL *curl = curl_easy_init();

  curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
  curl_easy_setopt(curl, CURLOPT_URL, "https://api.github.com/gists");

  struct curl_slist *headers = NULL;
  headers = curl_slist_append(headers, "cache-control: no-cache");
  headers = curl_slist_append(headers, "User-Agent: webFS");
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

  char *encoded_output;
  base64_encode(buf, strlen(buf), &encoded_output);
  char *template = "{\"description\": \"\", \"public\": false, \"files\": { \"webfs\": { \"content\": \"%s\"}}}";
  size_t total_length = strlen(template) + strlen(encoded_output);
  char *req_json = malloc(sizeof(char) * total_length);
  sprintf(req_json, template, encoded_output);

  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, req_json);

  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);

  struct string s;
  init_string(&s);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
  CURLcode ret = curl_easy_perform(curl);
  if (ret != CURLE_OK) {
    printf("errore");
  }
  json_value *json = json_parse(s.ptr, s.len);

  json_value *file = get_json_object_value_at_key(get_json_object_value_at_key(get_json_object_value_at_key(json, "files"), "webfs"), "raw_url");
  if (file == NULL) {
    return NULL;
  }
 
  return file->u.string.ptr;
}

char *storage_read(const char *location)
{
  CURL *curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
  struct curl_slist *headers = NULL;
  headers = curl_slist_append(headers, "cache-control: no-cache");
  headers = curl_slist_append(headers, "User-Agent: webFS");

  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(curl, CURLOPT_URL, location);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
  struct string s;
  init_string(&s);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
  CURLcode ret = curl_easy_perform(curl);

  unsigned char *decoded_file_contents;
  size_t decoded_length;
  base64_decode(s.ptr, &decoded_file_contents, &decoded_length);
  return (char *)decoded_file_contents;
}

