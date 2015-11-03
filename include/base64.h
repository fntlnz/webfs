#ifndef BASE64_H
#define BASE64_H
int base64_decode(char* b64message, unsigned char** buffer, size_t* length);
int base64_encode(const char* buffer, size_t length, char** b64text);
#endif // BASE64_H
