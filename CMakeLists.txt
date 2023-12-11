cmake_minimum_required(VERSION 3.27)
include(FetchContent)
include(CMake/JSON.cmake)

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

add_subdirectory(Glow)

# if(ENABLE_GLOW_ICONCONVERTER) add_subdirectory( Tools/IconConverter EXCLUDE_FROM_ALL ) endif()

# if(ENABLE_GLOW_EXAMPLES) add_subdirectory(Examples) endif()

# if(ENABLE_GLOW_TESTING) add_subdirectory( Testing EXCLUDE_FROM_ALL ) endif()
