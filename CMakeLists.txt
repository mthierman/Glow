cmake_minimum_required(VERSION 3.26)
set(APP_NAME "Glow")
set(APP_VERSION "0.0.1")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${APP_NAME}")

include(FetchContent)

option(ENV_LIBS OFF)
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

set(WEBVIEW "Microsoft.Web.WebView2")
set(WEBVIEW_VERSION "1.0.1938.49")

set(CPPWINRT "Microsoft.Windows.CppWinRT")
set(CPPWINRT_VER "2.0.230706.1")

set(WIL "Microsoft.Windows.ImplementationLibrary")
set(WIL_VERSION "1.0.230824.2")

if(ENV_LIBS)
    file(TO_CMAKE_PATH "$ENV{LIBRARIES}/${WEBVIEW}.${WEBVIEW_VERSION}" WEBVIEW_PKG)
    file(TO_CMAKE_PATH "$ENV{LIBRARIES}/${CPPWINRT}.${CPPWINRT_VER}" CPPWINRT_PKG)
    file(TO_CMAKE_PATH "$ENV{LIBRARIES}/${WIL}.${WIL_VERSION}" WIL_PKG)
else()
    execute_process(
        COMMAND
        nuget install "${WEBVIEW}" -Version "${WEBVIEW_VERSION}" -OutputDirectory "${CMAKE_BINARY_DIR}/_deps"
    )
    file(TO_CMAKE_PATH "${CMAKE_BINARY_DIR}/_deps/${WEBVIEW}.${WEBVIEW_VERSION}" WEBVIEW_PKG)

    execute_process(
        COMMAND
        nuget install "${CPPWINRT}" -Version "${CPPWINRT_VER}" -OutputDirectory "${CMAKE_BINARY_DIR}/_deps"
    )
    file(TO_CMAKE_PATH "${CMAKE_BINARY_DIR}/_deps/${CPPWINRT}.${CPPWINRT_VER}" CPPWINRT_PKG)

    execute_process(
        COMMAND
        nuget install "${WIL}" -Version "${WIL_VERSION}" -OutputDirectory "${CMAKE_BINARY_DIR}/_deps"
    )
    file(TO_CMAKE_PATH "${CMAKE_BINARY_DIR}/_deps/${WIL}.${WIL_VERSION}" WIL_PKG)
endif()

message(STATUS "WebView2 ${WEBVIEW_PKG}")
message(STATUS "C++/WinRT ${CPPWINRT_PKG}")
message(STATUS "WIL ${WIL_PKG}")

execute_process(
    COMMAND cppwinrt -input Microsoft.Web.WebView2.Core.winmd sdk WORKING_DIRECTORY "${WEBVIEW_PKG}/lib"
)

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

    # "${WEBVIEW_PKG}/lib"
    "${WIL_PKG}/include"
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
    dwmapi.lib
)

# add_custom_command(
# TARGET ${APP_NAME} POST_BUILD
# COMMAND ${CMAKE_COMMAND} -E copy "${WEBVIEW_PKG}/runtimes/win-x64/native_uap/Microsoft.Web.WebView2.Core.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
# )
