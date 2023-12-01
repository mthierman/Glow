cmake_minimum_required(VERSION 3.27)

project(Glow VERSION 0.0.1 LANGUAGES C CXX)

option(GLOW_BUILD_EXAMPLES OFF)
option(GLOW_BUILD_ICONCONVERTER OFF)
option(GLOW_ENABLE_TESTING OFF)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# if (PROJECT_IS_TOP_LEVEL)
    set(CMAKE_NUGET_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/_deps/Nuget")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_INCLUDE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/_deps/Glow/include)
# endif()

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
    enable_testing()
    add_subdirectory(tests)
endif()
