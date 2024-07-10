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

add_library(Glow)

add_library(
    glow::glow
    ALIAS
    Glow
    )

configure_file(
    "${PROJECT_SOURCE_DIR}/src/config.hxx"
    "${CMAKE_CURRENT_BINARY_DIR}/include/glow/config.hxx"
    )

target_sources(
    Glow
    PRIVATE "${PROJECT_SOURCE_DIR}/src/app.cxx"
            "${PROJECT_SOURCE_DIR}/src/coinitialize.cxx"
            "${PROJECT_SOURCE_DIR}/src/colors.cxx"
            "${PROJECT_SOURCE_DIR}/src/console.cxx"
            "${PROJECT_SOURCE_DIR}/src/database.cxx"
            "${PROJECT_SOURCE_DIR}/src/filesystem.cxx"
            "${PROJECT_SOURCE_DIR}/src/gdiplus.cxx"
            "${PROJECT_SOURCE_DIR}/src/gui.cxx"
            "${PROJECT_SOURCE_DIR}/src/guid.cxx"
            "${PROJECT_SOURCE_DIR}/src/notification.cxx"
            "${PROJECT_SOURCE_DIR}/src/system.cxx"
            "${PROJECT_SOURCE_DIR}/src/text.cxx"
            "${PROJECT_SOURCE_DIR}/src/url.cxx"
            "${PROJECT_SOURCE_DIR}/src/webview.cxx"
            "${PROJECT_SOURCE_DIR}/src/window.cxx"
    PRIVATE FILE_SET
            HEADERS
            BASE_DIRS
            "${PROJECT_SOURCE_DIR}/src"
            "${PROJECT_SOURCE_DIR}/include"
            "${CMAKE_CURRENT_BINARY_DIR}/include"
            FILES
            "${PROJECT_SOURCE_DIR}/src/app.hxx"
            "${PROJECT_SOURCE_DIR}/src/coinitialize.hxx"
            "${PROJECT_SOURCE_DIR}/src/colors.hxx"
            "${PROJECT_SOURCE_DIR}/src/console.hxx"
            "${PROJECT_SOURCE_DIR}/src/database.hxx"
            "${PROJECT_SOURCE_DIR}/src/filesystem.hxx"
            "${PROJECT_SOURCE_DIR}/src/gdiplus.hxx"
            "${PROJECT_SOURCE_DIR}/src/gui.hxx"
            "${PROJECT_SOURCE_DIR}/src/guid.hxx"
            "${PROJECT_SOURCE_DIR}/src/notification.hxx"
            "${PROJECT_SOURCE_DIR}/src/position.hxx"
            "${PROJECT_SOURCE_DIR}/src/random.hxx"
            "${PROJECT_SOURCE_DIR}/src/system.hxx"
            "${PROJECT_SOURCE_DIR}/src/text.hxx"
            "${PROJECT_SOURCE_DIR}/src/url.hxx"
            "${PROJECT_SOURCE_DIR}/src/webview.hxx"
            "${PROJECT_SOURCE_DIR}/src/window.hxx"
            "${PROJECT_SOURCE_DIR}/include/glow/glow.hxx"
            "${CMAKE_CURRENT_BINARY_DIR}/include/glow/config.hxx"
    )

target_include_directories(
    Glow
    PUBLIC "${PROJECT_SOURCE_DIR}/include"
           "${CMAKE_CURRENT_BINARY_DIR}/include"
           "${webview2_SOURCE_DIR}/build/native/include-winrt"
           "${webview2_SOURCE_DIR}/build/native/include"
           "${wil_SOURCE_DIR}/include"
    )

target_link_directories(
    Glow
    PUBLIC
    "${webview2_SOURCE_DIR}/build/native/x64"
    )

target_link_libraries(
    Glow
    PRIVATE common::flags
    PUBLIC common::features
           common::definitions
           ms::wil
           ms::cppwinrt
           ms::webview2
           ada-url::ada
           nlohmann::json
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

if(GLOW_EXAMPLES)
    add_subdirectory(examples)
endif()

if(GLOW_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
