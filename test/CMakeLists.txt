find_package(Threads REQUIRED)

include(DownloadGoogleTest)
include(DownloadGoogleMock)
add_executable(webfstest main.cpp node_test.cpp)

include_directories(${COMMON_INCLUDES})

target_link_libraries(webfstest libwebfs libgtest libgmock)
