cmake_minimum_required(VERSION 3.30)

project(Glow VERSION 0.0.8)

option(
    GLOW_EXAMPLES
    "Build examples"
    OFF
    )

include(FetchContent)

FetchContent_Declare(
    cmake-modules
    GIT_REPOSITORY "https://github.com/mthierman/cmake-modules.git"
    GIT_TAG main
    GIT_SHALLOW ON
    )

FetchContent_MakeAvailable(cmake-modules)

list(
    APPEND
    CMAKE_MODULE_PATH
    "${cmake-modules_SOURCE_DIR}"
    )

include(cmake-modules)

fetch_common()

fetch_wil(
    VERSION
    1.0.240122.1
    )

fetch_cppwinrt(
    VERSION
    2.0.240405.15
    )

fetch_webview2(
    VERSION
    1.0.2592.51
    )

fetch_ada(
    VERSION
    2.8.0
    )

fetch_json(
    VERSION
    3.11.3
    )

fetch_sqlite(
    VERSION
    3460000
    )

make_release_info()

add_library(${PROJECT_NAME})

add_library(
    glow::glow
    ALIAS
    Glow
    )

target_include_directories(${PROJECT_NAME} PUBLIC "src/include")

target_sources(
    ${PROJECT_NAME}
    PRIVATE "src/color.cxx"
            "src/filesystem.cxx"
            "src/input.cxx"
            "src/math.cxx"
            "src/messages.cxx"
            "src/std.cxx"
            "src/system.cxx"
            "src/text.cxx"
            "src/webview.cxx"
            "src/webviewrt.cxx"
            "src/window.cxx"
    )

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE common::flags
    PUBLIC common::features
           common::definitions
           microsoft::wil
           microsoft::cppwinrt
           microsoft::webview2
           dwmapi.lib
           gdiplus.lib
           WindowsApp.lib
    )

if(GLOW_EXAMPLES)
    add_subdirectory(examples)
endif()
