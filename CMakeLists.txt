cmake_minimum_required(VERSION 3.30)

project(Glow VERSION 0.0.8)

option(
    BUILD_EXAMPLES
    "Build examples"
    OFF
    )

include(cmake/deps.cmake)

add_library(
    ${PROJECT_NAME}_compile_options
    INTERFACE
    )

add_library(
    glow::compile_options
    ALIAS
    ${PROJECT_NAME}_compile_options
    )

target_compile_options(
    ${PROJECT_NAME}_compile_options
    INTERFACE $<$<CXX_COMPILER_ID:MSVC>:
              /W4
              /WX
              /MP
              /nologo
              /utf-8
              /bigobj
              /diagnostics:caret
              /Zc:__cplusplus
              >
              $<$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>>:
              /W4
              /WX
              -Wno-braced-scalar-init
              -Wno-extra-semi
              -Wno-gnu-zero-variadic-macro-arguments
              -Wno-language-extension-token
              -Wno-microsoft-enum-value
              -Wno-nonportable-include-path
              >
              $<$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:GNU>>:
              -Wall
              -Werror
              -Wno-braced-scalar-init
              -Wno-extra-semi
              -Wno-gnu-zero-variadic-macro-arguments
              -Wno-language-extension-token
              -Wno-microsoft-enum-value
              -Wno-nonportable-include-path
              >
    )

add_library(${PROJECT_NAME})

add_library(
    glow::glow
    ALIAS
    ${PROJECT_NAME}
    )

target_sources(
    ${PROJECT_NAME}
    PRIVATE "src/app.cxx"
            "src/color.cxx"
            "src/config.cxx"
            "src/filesystem.cxx"
            "src/guid.cxx"
            "src/input.cxx"
            "src/log.cxx"
            "src/math.cxx"
            "src/message.cxx"
            "src/system.cxx"
            "src/text.cxx"
            # "src/webview.cxx" "src/webviewrt.cxx"
            "src/window.cxx"
    PUBLIC FILE_SET
           HEADERS
           BASE_DIRS
           "src/include"
    )

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE common::compile_features
            common::compile_definitions
            common::link_options_lib
            glow::compile_options
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
