cmake_minimum_required(VERSION 3.26)
set(APP_NAME "Glow")
set(APP_VERSION "0.0.1")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${APP_NAME}")

project(
    ${APP_NAME}
    VERSION ${APP_VERSION}
    LANGUAGES C CXX ASM
)

math(EXPR BITNESS "${CMAKE_SIZEOF_VOID_P} * 8" OUTPUT_FORMAT DECIMAL)
message(STATUS "${APP_NAME} v${APP_VERSION} (${BITNESS}-bit) [${CMAKE_BUILD_TYPE}]")

file(GLOB SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cxx")
file(TO_CMAKE_PATH "${CMAKE_SOURCE_DIR}/app.manifest" MANIFEST_FILE)

add_executable(
    ${APP_NAME}
    WIN32
    "${SOURCES}"
    "${MANIFEST_FILE}"
)

add_compile_definitions(
    UNICODE
    APP_NAME="${APP_NAME}"
    APP_VERSION="${APP_VERSION}"
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

target_link_libraries(
    ${APP_NAME}
    PRIVATE
    dwmapi.lib

    # gdiplus.lib
    # shcore.lib
)
