find_package(Threads REQUIRED)
#use googletest library
include(DownloadGoogleTest)
include(DownloadGoogleMock)

add_executable(libwebfstest main.cpp node_test.cpp filesystem_test.cpp)
#reuqire the rapidjson library
add_dependencies(libwebfstest rapidjson)

include_directories(${COMMON_INCLUDES})


target_link_libraries(libwebfstest libwebfs libgtest libgmock)
