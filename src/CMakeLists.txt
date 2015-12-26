#create the libwebfs library

find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)

include_directories(${COMMON_INCLUDES} ${FUSE_INCLUDE_DIR} ${CURL_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})

set(SOURCE_FILES node.cpp utils.cpp filesystem.cpp node_serializer.cpp storage/gist.cpp)

add_library(libwebfs STATIC ${SOURCE_FILES})
#require the rapidjson library
add_dependencies(libwebfs rapidjson)

target_link_libraries(libwebfs ${CURL_LIBRARIES} ${OPENSSL_LIBRARIES})
set_target_properties(libwebfs PROPERTIES PREFIX "")
