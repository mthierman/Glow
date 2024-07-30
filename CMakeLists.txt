cmake_minimum_required(VERSION 3.30)

project(Glow VERSION 0.0.8)

option(
    BUILD_EXAMPLES
    "Build examples"
    OFF
    )

include(cmake/deps.cmake)

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
    PRIVATE common::compile_features
            common::compile_definitions
            common::compile_options
            common::link_options_lib
    PUBLIC microsoft::wil
           microsoft::cppwinrt
           microsoft::webview2
           dwmapi.lib
           gdiplus.lib
           WindowsApp.lib
    )

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
