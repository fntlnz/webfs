find_package(FUSE REQUIRED)
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)

include_directories(${COMMON_INCLUDES} ${FUSE_INCLUDE_DIR} ${CURL_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})

set(SOURCE_FILES main.c fuse.c base64.c storage/gist.c tree.c)

add_executable(webfs ${SOURCE_FILES})
target_link_libraries(webfs ${FUSE_LIBRARIES} ${CURL_LIBRARIES} ${OPENSSL_LIBRARIES})
