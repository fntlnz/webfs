find_package(FUSE REQUIRED)
find_package(CURL REQUIRED)

set(JSONPARSER_FILES ${PROJECT_SOURCE_DIR}/vendor/json-parser/json.c)
set(JSONPARSER_INCLUDES ${PROJECT_SOURCE_DIR}/vendor/json-parser/json.h)

include_directories(${COMMON_INCLUDES} ${FUSE_INCLUDE_DIR} ${CURL_INCLUDE_DIRS} ${JSONPARSER_INCLUDES})

set(SOURCE_FILES fuse.c)
add_library(libwebfs STATIC ${SOURCE_FILES} ${JSONPARSER_FILES})

target_link_libraries(libwebfs ${FUSE_LIBRARIES} ${CURL_LIBRARIES})

add_executable(webfs main.c)
target_link_libraries(webfs libwebfs)
