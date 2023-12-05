cmake_minimum_required(VERSION 3.27)

list(
    APPEND
    CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/CMake
)

include(FetchContent)
include(PackageInfo)
include(Dependencies)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Config/GlowConfig.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/Config/GlowConfig.h
)

file(
    TO_CMAKE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/Data/Glow.manifest
    GLOW_DATA_MANIFEST
)

file(
    TO_CMAKE_PATH
    ${CMAKE_CURRENT_BINARY_DIR}/Config
    GLOW_CONFIG
)

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

add_library(
    glow_glow_common
    INTERFACE
)

add_library(
    glow::common
    ALIAS
    glow_glow_common
)

target_compile_features(
    glow_glow_common
    INTERFACE c_std_17
              cxx_std_23
)

target_compile_options(
    glow_glow_common
    INTERFACE $<$<CXX_COMPILER_ID:MSVC>:
              /W4
              /WX
              /bigobj
              /diagnostics:caret
              /W4
              /WX
              /wd4100
              /Zc:__cplusplus>
)

target_link_options(
    glow_glow_common
    INTERFACE
    $<$<CXX_COMPILER_ID:MSVC>:
    /WX>
)

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
