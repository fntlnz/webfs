# Download RapidJSON
include(ExternalProject)
ExternalProject_Add(
    rapidjson
    URL https://github.com/miloyip/rapidjson/archive/v1.0.2.zip
    PREFIX ${PROJECT_SOURCE_DIR}/vendor/rapidjson
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(rapidjson source_dir)
include_directories("${source_dir}/include")
#enable the std::strig wrapper function
add_definitions(-DRAPIDJSON_HAS_STDSTRING)
