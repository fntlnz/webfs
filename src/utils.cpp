#include <sstream>

#include "utils.h"

std::vector<std::string> webfs::utils::explode(std::string const & s, char delim) {
  std::vector<std::string> result;
  std::istringstream iss(s);

  for (std::string token; std::getline(iss, token, delim); ) {
    result.push_back(std::move(token));
  }

  return result;
}
