cmake_minimum_required(VERSION 3.27)

include(CMake/ReadJSON.cmake)

project(
    ${NAME}
    VERSION ${VERSION}
    LANGUAGES C
              CXX
)

include(FetchContent)
include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/Dependencies.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/Configure.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/Common.cmake)

add_subdirectory(libs/sqlite)
# add_subdirectory(libs/libcurl)

add_subdirectory(Glow)

if(ENABLE_GLOW_ICONCONVERTER)
    add_subdirectory(
        Tools/IconConverter
        EXCLUDE_FROM_ALL
    )
endif()

if(ENABLE_GLOW_EXAMPLES)
    add_subdirectory(Examples)
endif()

if(ENABLE_GLOW_TESTING)
    add_subdirectory(
        Testing
        EXCLUDE_FROM_ALL
    )
endif()
