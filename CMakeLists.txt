cmake_minimum_required(VERSION 3.26)
set(LIB_NAME "Glow")
set(LIB_VERSION "0.0.1")
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${LIB_NAME}")

include(FetchContent)

option(WEBVIEW_WINRT OFF)

project(
    ${LIB_NAME}
    VERSION ${LIB_VERSION}
    LANGUAGES C CXX ASM
)

set(WEBVIEW "Microsoft.Web.WebView2")
set(WEBVIEW_VERSION "1.0.2088.41")

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

add_library(common INTERFACE)

target_compile_definitions(
    common
    INTERFACE
    LIB_NAME="${LIB_NAME}"
    LIB_VERSION="${LIB_VERSION}"
)

target_link_libraries(
    common
    INTERFACE
    dwmapi.lib
    Comctl32.lib
    gdiplus.lib
)

if(WEBVIEW_WINRT)
    execute_process(
        COMMAND cppwinrt -input Microsoft.Web.WebView2.Core.winmd sdk WORKING_DIRECTORY "${WEBVIEW_PKG}/lib"
    )

    target_include_directories(
        common
        INTERFACE
        ${WEBVIEW_PKG}/lib
    )

    file(COPY "${WEBVIEW_PKG}/runtimes/win-x64/native_uap/Microsoft.Web.WebView2.Core.dll" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
else()
    target_include_directories(
        common
        INTERFACE
        ${WEBVIEW_PKG}/build/native/include
    )

    target_link_directories(
        common
        INTERFACE
        ${WEBVIEW_PKG}/build/native/x64
    )

    target_link_libraries(
        common
        INTERFACE
        WebView2LoaderStatic.lib
    )
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(
        common
        INTERFACE
        /await:strict
        /bigobj
        /diagnostics:caret
        /EHsc
        /permissive-
        /utf-8
        /W4
        /WX
        /Zc:__cplusplus
    )

    target_link_options(
        common
        INTERFACE
        /WX
    )
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(
        common
        INTERFACE
        -fcoroutines
    )
endif()

file(TO_CMAKE_PATH "${CMAKE_SOURCE_DIR}/data/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.rc" RC_FILE)
file(TO_CMAKE_PATH "${CMAKE_SOURCE_DIR}/data/app.manifest" MANIFEST_FILE)

add_executable(
    MainWindow
    WIN32
    ${CMAKE_SOURCE_DIR}/tests/MainWindow.cxx
    ${RC_FILE}
    ${MANIFEST_FILE}
)

set_property(
    TARGET MainWindow
    APPEND PROPERTY LINK_LIBRARIES common
)

if(WEBVIEW_WINRT)
    add_executable(
        Composition
        WIN32
        ${CMAKE_SOURCE_DIR}/tests/Composition.cxx
        ${RC_FILE}
        ${MANIFEST_FILE}
    )

    set_property(
        TARGET Composition
        APPEND PROPERTY LINK_LIBRARIES common
    )

    target_link_libraries(
        Composition
        PRIVATE
        WindowsApp.lib
    )
endif()
