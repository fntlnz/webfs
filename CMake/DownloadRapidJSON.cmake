# Download RapidJSON
include(ExternalProject)
ExternalProject_Add(
    rapidjson
    URL https://github.com/miloyip/rapidjson/archive/v1.0.2.zip
    PREFIX ${PROJECT_SOURCE_DIR}/vendor/rapidjson
    INSTALL_COMMAND ""
)

set(RAPIDJSON_INCLUDES ${PROJECT_SOURCE_DIR}/vendor/rapidjson/include)
