find_package(Threads REQUIRED)

include(DownloadGoogleTest)
include(DownloadGoogleMock)
add_executable(libwebfstest main.cpp node_test.cpp filesystem_test.cpp)

include_directories(${COMMON_INCLUDES})

target_link_libraries(libwebfstest libwebfs libgtest libgmock)
