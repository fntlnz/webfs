find_package(FUSE REQUIRED)
find_package(CURL REQUIRED)
include_directories(${COMMON_INCLUDES} ${FUSE_INCLUDE_DIR} ${CURL_INCLUDE_DIRS})

set(SOURCE_FILES fuse.c)
add_library(libwebfs STATIC ${SOURCE_FILES})

target_link_libraries(libwebfs ${FUSE_LIBRARIES} ${CURL_LIBRARIES})

add_executable(webfs main.cpp)
target_link_libraries(webfs libwebfs)
