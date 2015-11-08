find_package(FUSE REQUIRED)
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)

set(JSONPARSER_SOURCE_FILES ${PROJECT_SOURCE_DIR}/vendor/json-parser/json.c)
set(JSONPARSER_INCLUDES ${PROJECT_SOURCE_DIR}/vendor/json-parser)
include_directories(${COMMON_INCLUDES} ${FUSE_INCLUDE_DIR} ${CURL_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR} ${JSONPARSER_INCLUDES})

set(SOURCE_FILES main.c fuse.c base64.c storage/gist.c tree.c)

add_executable(webfs ${SOURCE_FILES} ${JSONPARSER_SOURCE_FILES})
target_link_libraries(webfs ${FUSE_LIBRARIES} ${CURL_LIBRARIES} ${OPENSSL_LIBRARIES} m)
