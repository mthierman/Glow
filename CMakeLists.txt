cmake_minimum_required(VERSION 3.27)

file(
    READ
    ${CMAKE_CURRENT_SOURCE_DIR}/Glow.json
    GLOW_JSON
)

string(
    JSON
    GLOW_VERSION
    GET
    ${GLOW_JSON}
    version
)

project(
    Glow
    VERSION ${GLOW_VERSION}
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

list(
    APPEND
    CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

include(Dependencies)
include(Data)

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
