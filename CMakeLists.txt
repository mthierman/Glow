cmake_minimum_required(VERSION 3.28)

project(
    Glow
    VERSION 0.0.6
)

option(
    GLOW_RESTORE_NUGET
    "Restore Nuget packages"
    ON
)

option(
    GLOW_BUILD_TESTING
    "Build testing"
    OFF
)

option(
    GLOW_BUILD_EXAMPLES
    "Build examples"
    OFF
)

option(
    GLOW_BUILD_TOOLS
    "Build tools"
    OFF
)

enable_testing()

include(cmake/ReleaseInfo.cmake)

include(cmake/Common.cmake)

if(GLOW_RESTORE_NUGET)
    execute_process(
        COMMAND nuget restore -PackagesDirectory "${CMAKE_BINARY_DIR}/_deps/Nuget"
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )

    cmake_path(
        SET
        webview2_SOURCE_DIR
        "${CMAKE_BINARY_DIR}/_deps/Nuget/Microsoft.Web.WebView2.1.0.2478.35"
    )

    cmake_path(
        SET
        cppwinrt_SOURCE_DIR
        "${CMAKE_BINARY_DIR}/_deps/Nuget/Microsoft.Windows.CppWinRT.2.0.240405.15"
    )

    cmake_path(
        SET
        wil_SOURCE_DIR
        "${CMAKE_BINARY_DIR}/_deps/Nuget/Microsoft.Windows.ImplementationLibrary.1.0.240122.1"
    )
endif()

execute_process(
    COMMAND
        cppwinrt -input "${webview2_SOURCE_DIR}/lib/Microsoft.Web.WebView2.Core.winmd" sdk -output
        "${webview2_SOURCE_DIR}/build/native/include-winrt"
    WORKING_DIRECTORY "${cppwinrt_SOURCE_DIR}/bin"
)

add_library(Ada)

add_library(
    ada::ada
    ALIAS
    Ada
)

target_sources(
    Ada
    PRIVATE "${PROJECT_SOURCE_DIR}/libs/ada-url/ada/ada.cpp"
    PUBLIC FILE_SET
           HEADERS
           BASE_DIRS
           "${PROJECT_SOURCE_DIR}/libs/ada-url/ada"
           FILES
           "${PROJECT_SOURCE_DIR}/libs/ada-url/ada/ada.h"
)

target_link_libraries(
    Ada
    PRIVATE glow::definitions
            glow::features
            glow::flags_deps
)

add_library(
    JSON
    INTERFACE
)

add_library(
    nlohmann_json::nlohmann_json
    ALIAS
    JSON
)

target_sources(
    JSON
    PUBLIC FILE_SET
           HEADERS
           BASE_DIRS
           "${PROJECT_SOURCE_DIR}/libs/nlohmann/json/single_include/nlohmann"
           FILES
           "${PROJECT_SOURCE_DIR}/libs/nlohmann/json/single_include/nlohmann/json.hpp"
           "${PROJECT_SOURCE_DIR}/libs/nlohmann/json/single_include/nlohmann/json_fwd.hpp"
)

add_library(SQLite)

add_library(
    sqlite::sqlite
    ALIAS
    SQLite
)

target_sources(
    SQLite
    PRIVATE "${PROJECT_SOURCE_DIR}/libs/sqlite/sqlite3.c"
    PUBLIC FILE_SET
           HEADERS
           BASE_DIRS
           "${PROJECT_SOURCE_DIR}/libs/sqlite/"
           FILES
           "${PROJECT_SOURCE_DIR}/libs/sqlite/sqlite3.h"
)

target_link_libraries(
    SQLite
    PRIVATE glow::definitions
            glow::features
            glow::flags_deps
)

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

if(GLOW_BUILD_TESTING)
    add_subdirectory(tests)
endif()

if(GLOW_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(GLOW_BUILD_TOOLS)
    add_subdirectory(tools)
endif()
