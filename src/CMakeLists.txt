find_package(FUSE REQUIRED)
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)

set(RAPIDJSON_INCLUDES ${PROJECT_SOURCE_DIR}/vendor/rapidjson/include)
include_directories(${COMMON_INCLUDES} ${FUSE_INCLUDE_DIR} ${CURL_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR} ${RAPIDJSON_INCLUDES})

set(SOURCE_FILES fuse.cpp node.cpp utils.cpp)

add_executable(webfs ${SOURCE_FILES})
target_link_libraries(webfs ${FUSE_LIBRARIES} ${CURL_LIBRARIES} ${OPENSSL_LIBRARIES} m)
