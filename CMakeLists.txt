cmake_minimum_required(VERSION 3.28)

project(Glow VERSION 0.0.7)

option(
    GLOW_TESTS
    "Build tests"
    OFF
    )

option(
    GLOW_EXAMPLES
    "Build examples"
    OFF
    )

option(
    GLOW_TOOLS
    "Build tools"
    OFF
    )

include(cmake/Common.cmake)
include(cmake/ReleaseInfo.cmake)
include(cmake/Nuget.cmake)
include(cmake/Ada.cmake)
include(cmake/JSON.cmake)
include(cmake/SQLite.cmake)

add_library(Glow)

add_library(
    glow::glow
    ALIAS
    Glow
    )

configure_file(
    "${PROJECT_SOURCE_DIR}/src/config.hxx"
    "${CMAKE_CURRENT_BINARY_DIR}/include/glow/config.hxx"
    )

target_sources(
    Glow
    PRIVATE "${PROJECT_SOURCE_DIR}/src/app.cxx"
            "${PROJECT_SOURCE_DIR}/src/coinitialize.cxx"
            "${PROJECT_SOURCE_DIR}/src/colors.cxx"
            "${PROJECT_SOURCE_DIR}/src/console.cxx"
            "${PROJECT_SOURCE_DIR}/src/database.cxx"
            "${PROJECT_SOURCE_DIR}/src/filesystem.cxx"
            "${PROJECT_SOURCE_DIR}/src/gdiplus.cxx"
            "${PROJECT_SOURCE_DIR}/src/gui.cxx"
            "${PROJECT_SOURCE_DIR}/src/guid.cxx"
            "${PROJECT_SOURCE_DIR}/src/notification.cxx"
            "${PROJECT_SOURCE_DIR}/src/system.cxx"
            "${PROJECT_SOURCE_DIR}/src/text.cxx"
            "${PROJECT_SOURCE_DIR}/src/url.cxx"
            "${PROJECT_SOURCE_DIR}/src/webview.cxx"
            "${PROJECT_SOURCE_DIR}/src/window.cxx"
    PRIVATE FILE_SET
            HEADERS
            BASE_DIRS
            "${PROJECT_SOURCE_DIR}/src"
            "${PROJECT_SOURCE_DIR}/include"
            "${CMAKE_CURRENT_BINARY_DIR}/include"
            FILES
            "${PROJECT_SOURCE_DIR}/src/app.hxx"
            "${PROJECT_SOURCE_DIR}/src/coinitialize.hxx"
            "${PROJECT_SOURCE_DIR}/src/colors.hxx"
            "${PROJECT_SOURCE_DIR}/src/console.hxx"
            "${PROJECT_SOURCE_DIR}/src/database.hxx"
            "${PROJECT_SOURCE_DIR}/src/filesystem.hxx"
            "${PROJECT_SOURCE_DIR}/src/gdiplus.hxx"
            "${PROJECT_SOURCE_DIR}/src/gui.hxx"
            "${PROJECT_SOURCE_DIR}/src/guid.hxx"
            "${PROJECT_SOURCE_DIR}/src/notification.hxx"
            "${PROJECT_SOURCE_DIR}/src/position.hxx"
            "${PROJECT_SOURCE_DIR}/src/random.hxx"
            "${PROJECT_SOURCE_DIR}/src/system.hxx"
            "${PROJECT_SOURCE_DIR}/src/text.hxx"
            "${PROJECT_SOURCE_DIR}/src/url.hxx"
            "${PROJECT_SOURCE_DIR}/src/webview.hxx"
            "${PROJECT_SOURCE_DIR}/src/window.hxx"
            "${PROJECT_SOURCE_DIR}/include/glow/glow.hxx"
            "${CMAKE_CURRENT_BINARY_DIR}/include/glow/config.hxx"
    )

target_include_directories(
    Glow
    PUBLIC "${PROJECT_SOURCE_DIR}/include"
           "${CMAKE_CURRENT_BINARY_DIR}/include"
           "${webview2_SOURCE_DIR}/build/native/include-winrt"
           "${webview2_SOURCE_DIR}/build/native/include"
           "${wil_SOURCE_DIR}/include"
    )

target_link_directories(
    Glow
    PUBLIC
    "${webview2_SOURCE_DIR}/build/native/x64"
    )

target_link_libraries(
    Glow
    PRIVATE glow::flags
    PUBLIC glow::definitions
           glow::features
           ada::ada
           nlohmann_json::nlohmann_json
           sqlite::sqlite
           dwmapi.lib
           Uxtheme.lib
           Comctl32.lib
           gdiplus.lib
           windowscodecs.lib
           ShLwApi.Lib
           WebView2LoaderStatic.lib
           WindowsApp.lib
    )

if(GLOW_EXAMPLES)
    add_subdirectory(examples)
endif()

if(GLOW_TOOLS)
    add_subdirectory(tools)
endif()

if(GLOW_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
