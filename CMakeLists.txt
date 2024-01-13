cmake_minimum_required(VERSION 3.27)

project(
    Glow
    VERSION 0.0.1
    LANGUAGES C
              CXX
)

set(WIL
    Microsoft.Windows.ImplementationLibrary
)
set(WIL_VERSION
    1.0.231216.1
)
set(CPPWINRT
    Microsoft.Windows.CppWinRT
)
set(CPPWINRT_VERSION
    2.0.230706.1
)
set(WEBVIEW2
    Microsoft.Web.WebView2
)
set(WEBVIEW2_VERSION
    1.0.2210.55
)

option(
    GLOW_LOCAL_LIBS
    OFF
)
option(
    GLOW_EXAMPLES
    OFF
)
option(
    GLOW_TESTS
    OFF
)
option(
    GLOW_TOOLS
    OFF
)

include(cmake/Common.cmake)

include(FetchContent)
# include(cmake/CURL.cmake)
include(cmake/SQLite.cmake)
include(cmake/JSON.cmake)
include(cmake/TOML.cmake)
include(cmake/WIL.cmake)
include(cmake/CPPWinRT.cmake)
include(cmake/WebView2.cmake)

include(cmake/BuildCPPWinRT.cmake)

add_library(Glow)

add_library(
    glow::glow
    ALIAS
    Glow
)

configure_file(
    ${PROJECT_SOURCE_DIR}/src/config.hxx
    ${CMAKE_CURRENT_BINARY_DIR}/config/glow/config.hxx
)

target_sources(
    Glow
    PRIVATE ${PROJECT_SOURCE_DIR}/src/console.cxx
            ${PROJECT_SOURCE_DIR}/src/filesystem.cxx
            ${PROJECT_SOURCE_DIR}/src/gui.cxx
            ${PROJECT_SOURCE_DIR}/src/text.cxx
    PUBLIC FILE_SET
           HEADERS
           BASE_DIRS
           ${PROJECT_SOURCE_DIR}/src
           ${PROJECT_SOURCE_DIR}/include
           ${CMAKE_CURRENT_BINARY_DIR}/config
           FILES
           ${CMAKE_CURRENT_BINARY_DIR}/config/glow/config.hxx
           ${PROJECT_SOURCE_DIR}/src/console.hxx
           ${PROJECT_SOURCE_DIR}/src/filesystem.hxx
           ${PROJECT_SOURCE_DIR}/src/gui.hxx
           ${PROJECT_SOURCE_DIR}/src/text.hxx
           ${PROJECT_SOURCE_DIR}/src/window.hxx
           ${PROJECT_SOURCE_DIR}/include/glow/glow.hxx
)

target_include_directories(
    Glow
    PUBLIC ${PROJECT_SOURCE_DIR}/include
           ${webview2_SOURCE_DIR}/build/native/include-winrt
           ${webview2_SOURCE_DIR}/build/native/include
           ${wil_SOURCE_DIR}/include
           ${sqlite_SOURCE_DIR}
           ${json_SOURCE_DIR}/single_include
           ${toml_SOURCE_DIR}/include
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
           tomlplusplus::tomlplusplus
           sqlite::sqlite
           dwmapi.lib
           Uxtheme.lib
           Comctl32.lib
           gdiplus.lib
           windowscodecs.lib
           ShLwApi.Lib
           WebView2LoaderStatic.lib
)

if(GLOW_EXAMPLES)
    add_subdirectory(examples)
endif()

if(GLOW_TESTS)
    add_subdirectory(tests)
endif()

if(GLOW_TOOLS)
    add_subdirectory(tools)
endif()
