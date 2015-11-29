# Download Google Mock
include(ExternalProject)
ExternalProject_Add(
    gmock
    URL https://googlemock.googlecode.com/files/gmock-1.7.0.zip
    PREFIX ${PROJECT_SOURCE_DIR}/vendor/gmock
    INSTALL_COMMAND ""
)

add_library(libgmock IMPORTED STATIC GLOBAL)
add_dependencies(libgmock gmock)

ExternalProject_Get_Property(gmock source_dir binary_dir)
set_target_properties(libgmock PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/libgmock.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)
include_directories("${source_dir}/include")
