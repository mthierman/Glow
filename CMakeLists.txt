cmake_minimum_required(VERSION 3.26)
set(APP_NAME "Glow")
set(APP_VERSION "0.0.1")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${APP_NAME}")

include(FetchContent)

option(ENV_LIBS OFF)
option(EXPERIMENTAL_LIBS OFF)
option(INSTALL OFF)
option(CREATE_SYMLINKS OFF)

project(
    ${APP_NAME}
    VERSION ${APP_VERSION}
    LANGUAGES C CXX ASM
)

math(EXPR BITNESS "${CMAKE_SIZEOF_VOID_P} * 8" OUTPUT_FORMAT DECIMAL)
message(STATUS "${APP_NAME} v${APP_VERSION} (${BITNESS}-bit) [${CMAKE_BUILD_TYPE}]")
file(WRITE "${CMAKE_BINARY_DIR}/app_version.txt" "v${APP_VERSION}")
execute_process(
    COMMAND
    git --no-pager log -5 --oneline --no-decorate
    OUTPUT_FILE "${CMAKE_BINARY_DIR}/release_notes.txt"
)

if(EXPERIMENTAL_LIBS)
    set(WEBVIEW "Microsoft.Web.WebView2")
    set(WEBVIEW_VERSION "1.0.1988-prerelease")
else()
    set(WEBVIEW "Microsoft.Web.WebView2")
    set(WEBVIEW_VERSION "1.0.1938.49")
endif()

if(ENV_LIBS)
    file(TO_CMAKE_PATH "$ENV{LIBRARIES}/${WEBVIEW}.${WEBVIEW_VERSION}" WEBVIEW_PKG)
else()
    execute_process(
        COMMAND
        nuget install "${WEBVIEW}" -Version "${WEBVIEW_VERSION}" -OutputDirectory "${CMAKE_BINARY_DIR}/_deps"
    )
    file(TO_CMAKE_PATH "${CMAKE_BINARY_DIR}/_deps/${WEBVIEW}.${WEBVIEW_VERSION}" WEBVIEW_PKG)
endif()

message(STATUS "WebView2 ${WEBVIEW_PKG}")

file(GLOB SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cxx")
file(TO_CMAKE_PATH "${CMAKE_SOURCE_DIR}/res$<$<CONFIG:Debug>:_debug>$<$<CONFIG:Release>:_release>.rc" RC_FILE)
file(TO_CMAKE_PATH "${CMAKE_SOURCE_DIR}/app.manifest" MANIFEST_FILE)

add_executable(
    ${APP_NAME}
    WIN32
    "${SOURCES}"
    "${RC_FILE}"
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

target_include_directories(
    ${APP_NAME}
    PRIVATE
    "${WEBVIEW_PKG}/build/native/include"
)

target_link_directories(
    ${APP_NAME}
    PRIVATE
    "${WEBVIEW_PKG}/build/native/x64"
)

target_link_libraries(
    ${APP_NAME}
    PRIVATE
    WebView2LoaderStatic.lib
)
