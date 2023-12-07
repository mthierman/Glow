cmake_minimum_required(VERSION 3.27)

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

include(FetchContent)
include(Dependencies)
include(Configure)
include(Common)

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
