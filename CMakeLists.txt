cmake_minimum_required(VERSION 3.27)

list(
    APPEND
    CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/CMake
)

include(FetchContent)
include(PackageInfo)
include(Dependencies)
include(Data)

project(
    ${NAME}
    VERSION ${VERSION}
    LANGUAGES C
              CXX
)

option(
    GLOW_BUILD_EXAMPLES
    OFF
)
option(
    GLOW_BUILD_ICONCONVERTER
    OFF
)
option(
    GLOW_ENABLE_TESTING
    OFF
)

include(Common)

add_subdirectory(libs/sqlite)
# add_subdirectory(libs/libcurl)

add_subdirectory(Glow)

if(GLOW_BUILD_ICONCONVERTER)
    add_subdirectory(Tools/IconConverter)
endif()

if(GLOW_BUILD_EXAMPLES)
    add_subdirectory(Examples)
endif()

if(GLOW_ENABLE_TESTING)
    add_subdirectory(Testing)
endif()
