cmake_minimum_required(VERSION 3.30)

project(Glow VERSION 0.1.0)

option(
    BUILD_EXAMPLES
    "Build examples"
    OFF
    )

include(cmake/dependencies.cmake)

add_library(${PROJECT_NAME})

add_library(
    glow::glow
    ALIAS
    ${PROJECT_NAME}
    )

target_sources(
    ${PROJECT_NAME}
    PRIVATE "src/color.cxx"
            "src/config.cxx"
            "src/filesystem.cxx"
            "src/guid.cxx"
            "src/input.cxx"
            "src/log.cxx"
            "src/math.cxx"
            "src/message.cxx"
            "src/system.cxx"
            "src/text.cxx"
            "src/window.cxx"
    PUBLIC FILE_SET
           HEADERS
           BASE_DIRS
           "src/include"
    )

target_compile_features(
    ${PROJECT_NAME}
    PUBLIC c_std_17
           cxx_std_23
    )

target_compile_definitions(
    ${PROJECT_NAME}
    PRIVATE WIN32_LEAN_AND_MEAN
    PUBLIC NOMINMAX
           GDIPVER=0x0110
    )

target_compile_options(
    ${PROJECT_NAME}
    PRIVATE $<$<CXX_COMPILER_ID:MSVC>:
            /MP
            >
            $<$<CXX_COMPILER_ID:Clang>:
            -Wno-braced-scalar-init
            -Wno-nonportable-include-path
            >
            $<$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>:
            /W4
            /WX
            /nologo
            /utf-8
            /bigobj
            /diagnostics:caret
            /Zc:__cplusplus
            >
            $<$<CXX_COMPILER_FRONTEND_VARIANT:GNU>:
            -Wall
            -Werror
            >
    )

target_link_options(
    ${PROJECT_NAME}
    PRIVATE
    $<$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>:
    /WX
    /NOLOGO
    >
    $<$<CXX_COMPILER_FRONTEND_VARIANT:GNU>:
    -Wl,/WX,/NOLOGO
    >
    )

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC microsoft::wil
           microsoft::cppwinrt
           microsoft::webview2
           WindowsApp.lib
           dwmapi.lib
           gdiplus.lib
    )

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
