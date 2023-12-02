cmake_minimum_required(VERSION 3.27)

list(
    APPEND
    CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

include(ReadJSON)
include(PackageInfo)

project(
    ${NAME}
    VERSION ${VERSION}
    LANGUAGES C
              CXX
)

set(NUGET_DIRECTORY
    ${CMAKE_BINARY_DIR}/_deps/Nuget
)

add_subdirectory(Glow)

if(GLOW_BUILD_ICONCONVERTER)
    add_subdirectory(tools/IconConverter)
endif()

if(GLOW_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(GLOW_ENABLE_TESTING)
    add_subdirectory(tests)
endif()
