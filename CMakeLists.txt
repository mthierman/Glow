cmake_minimum_required(VERSION 3.27)

project(
    Glow
    VERSION 0.0.1
    LANGUAGES C
              CXX
)

option(
    GLOW_RESTORE_NUGET
    "Restore Nuget packages"
    OFF
)

option(
    GLOW_EXTRAS
    "Build Glow examples, tests & tools"
    OFF
)

include(cmake/Common.cmake)

include(FetchContent)

FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
    sqlite
    URL https://www.sqlite.org/2024/sqlite-amalgamation-3450000.zip
)
FetchContent_MakeAvailable(sqlite)

if(GLOW_RESTORE_NUGET)
    execute_process(
        COMMAND nuget restore -PackagesDirectory ${CMAKE_BINARY_DIR}/_deps/Nuget
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )

    cmake_path(
        SET
        webview2_SOURCE_DIR
        "${CMAKE_BINARY_DIR}/_deps/Nuget/Microsoft.Web.WebView2.1.0.2210.55"
    )

    cmake_path(
        SET
        wil_SOURCE_DIR
        "${CMAKE_BINARY_DIR}/_deps/Nuget/Microsoft.Windows.ImplementationLibrary.1.0.240122.1"
    )

    cmake_path(
        SET
        cppwinrt_SOURCE_DIR
        "${CMAKE_BINARY_DIR}/_deps/Nuget/Microsoft.Windows.CppWinRT.2.0.240111.5"
    )
endif()

execute_process(
    COMMAND
        cppwinrt -input ${webview2_SOURCE_DIR}/lib/Microsoft.Web.WebView2.Core.winmd sdk -output
        ${webview2_SOURCE_DIR}/build/native/include-winrt
    WORKING_DIRECTORY ${cppwinrt_SOURCE_DIR}/bin
)

add_library(SQLite)

add_library(
    sqlite::sqlite
    ALIAS
    SQLite
)

target_sources(
    SQLite
    PRIVATE ${sqlite_SOURCE_DIR}/sqlite3.c
    PUBLIC FILE_SET
           HEADERS
           BASE_DIRS
           ${sqlite_SOURCE_DIR}
           FILES
           ${sqlite_SOURCE_DIR}/sqlite3.h
)

target_link_libraries(
    SQLite
    PRIVATE glow::features
            glow::flags
)

add_library(Glow)

add_library(
    glow::glow
    ALIAS
    Glow
)

configure_file(
    ${PROJECT_SOURCE_DIR}/src/config.hxx
    ${CMAKE_CURRENT_BINARY_DIR}/include/glow/config.hxx
)

target_sources(
    Glow
    PRIVATE ${PROJECT_SOURCE_DIR}/src/coinitialize.cxx
            ${PROJECT_SOURCE_DIR}/src/colors.cxx
            ${PROJECT_SOURCE_DIR}/src/console.cxx
            ${PROJECT_SOURCE_DIR}/src/database.cxx
            ${PROJECT_SOURCE_DIR}/src/filesystem.cxx
            ${PROJECT_SOURCE_DIR}/src/gdiplus.cxx
            ${PROJECT_SOURCE_DIR}/src/gui.cxx
            ${PROJECT_SOURCE_DIR}/src/guid.cxx
            ${PROJECT_SOURCE_DIR}/src/system.cxx
            ${PROJECT_SOURCE_DIR}/src/text.cxx
    PRIVATE FILE_SET
            HEADERS
            BASE_DIRS
            ${PROJECT_SOURCE_DIR}/src
            ${PROJECT_SOURCE_DIR}/include
            ${CMAKE_CURRENT_BINARY_DIR}/include
            FILES
            ${PROJECT_SOURCE_DIR}/src/coinitialize.hxx
            ${PROJECT_SOURCE_DIR}/src/colors.hxx
            ${PROJECT_SOURCE_DIR}/src/console.hxx
            ${PROJECT_SOURCE_DIR}/src/database.hxx
            ${PROJECT_SOURCE_DIR}/src/filesystem.hxx
            ${PROJECT_SOURCE_DIR}/src/gdiplus.hxx
            ${PROJECT_SOURCE_DIR}/src/gui.hxx
            ${PROJECT_SOURCE_DIR}/src/guid.hxx
            ${PROJECT_SOURCE_DIR}/src/notification.hxx
            ${PROJECT_SOURCE_DIR}/src/position.hxx
            ${PROJECT_SOURCE_DIR}/src/system.hxx
            ${PROJECT_SOURCE_DIR}/src/text.hxx
            ${PROJECT_SOURCE_DIR}/src/window.hxx
            ${PROJECT_SOURCE_DIR}/include/glow/glow.hxx
            ${CMAKE_CURRENT_BINARY_DIR}/include/glow/config.hxx
)

target_include_directories(
    Glow
    PUBLIC ${PROJECT_SOURCE_DIR}/include
           ${CMAKE_CURRENT_BINARY_DIR}/include
           ${webview2_SOURCE_DIR}/build/native/include-winrt
           ${webview2_SOURCE_DIR}/build/native/include
           ${wil_SOURCE_DIR}/include
           ${sqlite_SOURCE_DIR}
           ${json_SOURCE_DIR}/single_include
)

target_link_directories(
    Glow
    PUBLIC
    ${webview2_SOURCE_DIR}/build/native/x64
)

target_link_libraries(
    Glow
    PRIVATE glow::flags
    PUBLIC glow::features
           glow::definitions
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

if(GLOW_EXTRAS)
    include(CTest)
    enable_testing()
    add_subdirectory(examples)
    add_subdirectory(tests)
    add_subdirectory(tools)
endif()
