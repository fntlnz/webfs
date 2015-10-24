# Download JsonCpp
include(ExternalProject)
ExternalProject_Add(
        jsoncpp
        URL https://github.com/open-source-parsers/jsoncpp/archive/0.10.5.zip
        PREFIX ${PROJECT_SOURCE_DIR}/vendor/jsoncpp
        INSTALL_COMMAND ""
)

set(JSONCPP_INCLUDES ${PROJECT_SOURCE_DIR}/vendor/jsoncpp/src/jsoncpp/dist)
set(JSONCPP_FILES ${PROJECT_SOURCE_DIR}/vendor/jsoncpp/src/jsoncpp/dist/jsoncpp.cpp)
