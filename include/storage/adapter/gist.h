#include "storage/adapter/adapter.h"

#ifndef WEBFS_GIST_H
#define WEBFS_GIST_H


struct string {
    char *ptr;
    size_t len;
};

namespace Storage {
    namespace Adapter {
        class Gist : public Adapter {
        public:
            Result write();
        private:
            static size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s);
        };
    }
}

#endif //WEBFS_GIST_H
