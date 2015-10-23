#include "storage/adapter/result.h"
#ifndef WEBFS_ADAPTER_H
#define WEBFS_ADAPTER_H

namespace Storage {
namespace Adapter {
class Adapter {
 public:
  virtual Result write() = 0;
};
}
}


#endif //WEBFS_ADAPTER_H
