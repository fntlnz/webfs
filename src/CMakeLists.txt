find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)

include(DownloadRapidJSON)

include_directories(${COMMON_INCLUDES} ${FUSE_INCLUDE_DIR} ${CURL_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR} ${RAPIDJSON_INCLUDES})

set(SOURCE_FILES node.cpp utils.cpp)

add_library(libwebfs STATIC ${SOURCE_FILES})

target_link_libraries(libwebfs ${CURL_LIBRARIES} ${OPENSSL_LIBRARIES})
set_target_properties(libwebfs PROPERTIES PREFIX "")
