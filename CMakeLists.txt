cmake_minimum_required(VERSION 3.29)

project(Glow VERSION 0.0.8)

option(
    GLOW_TESTS
    "Build tests"
    OFF
    )

option(
    GLOW_EXAMPLES
    "Build examples"
    OFF
    )

list(
    APPEND
    CMAKE_MODULE_PATH
    "${PROJECT_SOURCE_DIR}/libs/cmake-modules/"
    )

include(cmake-modules)

install_common()
install_wil(1.0.240122.1)
install_cppwinrt(2.0.240405.15)
install_webview2(1.0.2592.51)
install_ada(2.8.0)
install_json(3.11.3)
install_sqlite(3460000)
generate_cppwinrt()
release_info()

add_library(${PROJECT_NAME})

add_library(
    glow::glow
    ALIAS
    Glow
    )

target_include_directories(${PROJECT_NAME} PUBLIC "src/include")

target_sources(
    ${PROJECT_NAME}
    PRIVATE "src/filesystem.cxx"
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
    PUBLIC common::features
           common::definitions
           common::flags
           ms::wil
           ms::cppwinrt
           ms::webview2
           dwmapi.lib
           gdiplus.lib
           WindowsApp.lib
    )

if(GLOW_EXAMPLES)
    add_subdirectory(examples)
endif()

if(GLOW_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
