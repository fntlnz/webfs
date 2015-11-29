find_package(FUSE REQUIRED)
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)

include(DownloadRapidJSON)

include_directories(${COMMON_INCLUDES} ${FUSE_INCLUDE_DIR} ${CURL_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR} ${RAPIDJSON_INCLUDES})

set(SOURCE_FILES fuse.cpp node.cpp utils.cpp)

add_library(libwebfs STATIC ${SOURCE_FILES})

target_link_libraries(libwebfs ${FUSE_LIBRARIES} ${CURL_LIBRARIES} ${OPENSSL_LIBRARIES} m)

add_executable(webfs main.cpp)
target_link_libraries(webfs libwebfs)
