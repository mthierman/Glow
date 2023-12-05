cmake_minimum_required(VERSION 3.27)
include(FetchContent)

list(
    APPEND
    CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/CMake
)

include(ReadJSON)

project(
    ${NAME}
    VERSION ${VERSION}
    LANGUAGES C
              CXX
)

include(Dependencies)
include(Configure)
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
