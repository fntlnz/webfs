find_package(FUSE REQUIRED)

include_directories(${COMMON_INCLUDES} ${FUSE_INCLUDE_DIR})

add_executable(webfs main.cpp)
target_link_libraries(webfs libwebfs ${FUSE_LIBRARIES})
