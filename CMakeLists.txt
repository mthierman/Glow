cmake_minimum_required(VERSION 3.27)

project(
    Glow
    VERSION 0.0.0
    LANGUAGES C
              CXX
)

set(WIL
    Microsoft.Windows.ImplementationLibrary
)

set(WIL_VER
    1.0.231028.1
)

set(CPPWINRT
    Microsoft.Windows.CppWinRT
)

set(CPPWINRT_VER
    2.0.230706.1
)

set(WEBVIEW2
    Microsoft.Web.WebView2
)

set(WEBVIEW2_VER
    1.0.2151.40
)

option(
    ENABLE_GLOW_ICONCONVERTER
    OFF
)
option(
    ENABLE_GLOW_EXAMPLES
    OFF
)
option(
    ENABLE_GLOW_TESTING
    OFF
)
option(
    ENABLE_GLOW_GENERATE_EXAMPLE_ICONS
    OFF
)

include(FetchContent)
# include(cmake/libcurl.cmake)
include(cmake/common.cmake)
include(cmake/sqlite.cmake)
include(cmake/json.cmake)
include(cmake/toml.cmake)
include(cmake/wil.cmake)
include(cmake/cppwinrt.cmake)
include(cmake/webview2.cmake)

execute_process(
    COMMAND
        cppwinrt -input
        ${CMAKE_BINARY_DIR}/_deps/Nuget/${WEBVIEW2}.${WEBVIEW2_VER}/lib/Microsoft.Web.WebView2.Core.winmd
        sdk -output
        ${CMAKE_BINARY_DIR}/_deps/Nuget/${WEBVIEW2}.${WEBVIEW2_VER}/build/native/include-winrt
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/_deps/Nuget/${CPPWINRT}.${CPPWINRT_VER}/bin
)

add_subdirectory(glow)

if(ENABLE_GLOW_ICONCONVERTER)
    add_subdirectory(
        tools/IconConverter
        EXCLUDE_FROM_ALL
    )
endif()

if(ENABLE_GLOW_EXAMPLES)
    add_subdirectory(examples)
endif()

if(ENABLE_GLOW_TESTING)
    add_subdirectory(
        testing
        EXCLUDE_FROM_ALL
    )
endif()
