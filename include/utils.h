#include <vector>
#include <string>
#ifndef WEBFS_UTILS_H_
#define WEBFS_UTILS_H_
namespace webfs {
namespace utils {
std::vector<std::string> explode(std::string const & s, char delim);
} // utils namespace
} // webfs namespace
#endif // WEBFS_UTILS_H_
