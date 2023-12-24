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
    GLOW_TESTING
    OFF
)
option(
    GLOW_ICONCONVERTER
    OFF
)

include(cmake/common.cmake)

include(FetchContent)
# include(cmake/curl.cmake)
include(cmake/sqlite.cmake)
include(cmake/json.cmake)
include(cmake/toml.cmake)
include(cmake/wil.cmake)
include(cmake/cppwinrt.cmake)
include(cmake/webview2.cmake)

include(cmake/buildcppwinrt.cmake)

add_subdirectory(sqlite)

add_subdirectory(glow)

if(GLOW_EXAMPLES)
    add_subdirectory(examples)
endif()

if(GLOW_TESTING)
    add_subdirectory(
        testing
        EXCLUDE_FROM_ALL
    )
endif()

if(GLOW_ICONCONVERTER)
    add_subdirectory(
        tools/IconConverter
        EXCLUDE_FROM_ALL
    )
endif()
