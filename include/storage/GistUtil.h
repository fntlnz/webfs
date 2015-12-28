#ifndef INCLUDE_STORAGE_GISTUTIL_H_
#define INCLUDE_STORAGE_GISTUTIL_H_

#include <string>
#include "rapidjson/document.h"
#include "CurlUtil.h"

namespace webfs{
namespace storage{

class GistUtil{
public:
	static std::string getAuthToken(const std::string &user,const std::string &pass,
			const std::string &tokenNote,const std::string &userAgent="WebFs");

};

}}

#endif /* INCLUDE_STORAGE_GISTUTIL_H_ */
