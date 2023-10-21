cmake_minimum_required(VERSION 3.26)
set(APP_NAME "Glow")
set(APP_VERSION "0.0.0")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${APP_NAME}")

include(FetchContent)

project(
    ${APP_NAME}
    VERSION ${APP_VERSION}
    LANGUAGES C CXX ASM
)

file(GLOB SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cxx)
file(TO_CMAKE_PATH ${CMAKE_SOURCE_DIR}/res$<$<CONFIG:Debug>:_debug>$<$<CONFIG:Release>:_release>.rc RC_FILE)
file(TO_CMAKE_PATH ${CMAKE_SOURCE_DIR}/app.manifest MANIFEST_FILE)

add_executable(
    ${APP_NAME}
    WIN32
    ${SOURCES}
    ${RC_FILE}
    ${MANIFEST_FILE}
)

add_compile_definitions(
    UNICODE
    APP_NAME="${APP_NAME}"
    APP_VERSION="${APP_VERSION}"
)

target_link_libraries(
    ${APP_NAME}
    PRIVATE
    dwmapi.lib
    Comctl32.lib
)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(
        /EHsc
        /Zc:__cplusplus
        /await:strict
        /bigobj
        /permissive-
        /utf-8
        /W4
        /WX
    )

    add_link_options(
        /WX
    )
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(
        -fcoroutines
    )
endif()
