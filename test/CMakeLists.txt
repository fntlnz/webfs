find_package(Threads REQUIRED)

include(DownloadGoogleTest)
include(DownloadGoogleMock)

add_executable(webfstest main.cpp storage/adapter/gist_adapter_test.cpp)

include_directories(${COMMON_INCLUDES} ${GTEST_INCLUDES} ${GMOCK_INCLUDES})

target_link_libraries(webfstest
        libwebfs
        libgtest
        libgmock
)
