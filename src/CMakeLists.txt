find_package(FUSE REQUIRED)
find_package(CURL REQUIRED)
include(DownloadJsonCpp)

include_directories(${COMMON_INCLUDES} ${FUSE_INCLUDE_DIR} ${CURL_INCLUDE_DIRS} ${JSONCPP_INCLUDES})

set(SOURCE_FILES fuse.cpp storage/adapter/gist.cpp)
add_library(libwebfs STATIC ${SOURCE_FILES} ${JSONCPP_FILES})

target_link_libraries(libwebfs ${FUSE_LIBRARIES} ${CURL_LIBRARIES})

add_executable(webfs main.cpp)
target_link_libraries(webfs libwebfs)
