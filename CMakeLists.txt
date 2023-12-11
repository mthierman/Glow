cmake_minimum_required(VERSION 3.27)

include(FetchContent)
include(cmake/json.cmake)

project(
    ${NAME}
    VERSION ${VERSION}
    LANGUAGES C
              CXX
)

option(
    ENABLE_GLOW_ICONCONVERTER
    OFF
)
option(
    ENABLE_GLOW_EXAMPLES
    OFF
)
option(
    ENABLE_GLOW_TESTING
    OFF
)
option(
    ENABLE_GLOW_GENERATE_EXAMPLE_ICONS
    OFF
)

add_subdirectory(libs)

add_subdirectory(glow)

if(ENABLE_GLOW_ICONCONVERTER)
    add_subdirectory(
        tools/IconConverter
        EXCLUDE_FROM_ALL
    )
endif()

if(ENABLE_GLOW_EXAMPLES)
    add_subdirectory(examples)
endif()

if(ENABLE_GLOW_TESTING)
    add_subdirectory(
        testing
        EXCLUDE_FROM_ALL
    )
endif()
