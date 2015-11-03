#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <stdlib.h>

#include "storage/gist.h"
#include "base64.h"

struct string {
  char *ptr;
  size_t len;
};

void init_string(struct string *s)
{
  s->len = 0;
  s->ptr = (char *) malloc(s->len + 1);
  if (s->ptr == NULL) {
    fprintf(stderr, "malloc() failed\n");
    exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
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
  char *template = "{\"description\": \"\", \"public\": false, \"files\": { \"webfs.txt\": { \"content\": \"%s\"}}}";
  size_t total_length = strlen(template) + strlen(encoded_output);
  char *req_json = malloc(sizeof(char) * total_length);
  sprintf(req_json, template, encoded_output);

  //TODO: check here for memory allocation and sprintf
  // encrypt the file with openssl
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, req_json);

  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);

  struct string s;
  init_string(&s);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
  CURLcode ret = curl_easy_perform(curl);
  return s.ptr;
}

