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
    GLOW_TESTING
    OFF
)
option(
    GLOW_ICONCONVERTER
    OFF
)

include(CMake/Common.cmake)

include(FetchContent)
# include(CMake/CURL.cmake)
include(CMake/SQLite.cmake)
include(CMake/JSON.cmake)
include(CMake/TOML.cmake)
include(CMake/WIL.cmake)
include(CMake/CPPWinRT.cmake)
include(CMake/WebView2.cmake)

include(CMake/BuildCPPWinRT.cmake)

add_subdirectory(Glow)

if(GLOW_EXAMPLES)
    add_subdirectory(Examples)
endif()

if(GLOW_TESTING)
    add_subdirectory(
        Testing
        EXCLUDE_FROM_ALL
    )
endif()

if(GLOW_ICONCONVERTER)
    add_subdirectory(Tools/IconConverter)
endif()
