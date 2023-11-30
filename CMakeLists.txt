cmake_minimum_required(VERSION 3.26)

set(LIB_NAME "Glow")
set(LIB_VERSION "0.0.1")

project(${LIB_NAME} VERSION ${LIB_VERSION} LANGUAGES C CXX)

option(GLOW_BUILD_EXAMPLES OFF)
option(GLOW_BUILD_ICONCONVERTER OFF)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

if (PROJECT_IS_TOP_LEVEL)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/libs)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${LIB_NAME}_artifacts)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${LIB_NAME}_artifacts)
endif()

set(
    GLOW_HEADERS
    ${PROJECT_SOURCE_DIR}/glow/console/console.hxx
    ${PROJECT_SOURCE_DIR}/glow/filesystem/filesystem.hxx
    ${PROJECT_SOURCE_DIR}/glow/gui/app.hxx
    ${PROJECT_SOURCE_DIR}/glow/gui/gui.hxx
    ${PROJECT_SOURCE_DIR}/glow/gui/webview_composition.hxx
    ${PROJECT_SOURCE_DIR}/glow/gui/webview.hxx
    ${PROJECT_SOURCE_DIR}/glow/logging/logging.hxx
    ${PROJECT_SOURCE_DIR}/glow/text/text.hxx
)

include(Dependencies)

configure_file(
    ${PROJECT_SOURCE_DIR}/config/GlowConfig.h.in
    ${PROJECT_BINARY_DIR}/include/GlowConfig.h
)
file(TO_CMAKE_PATH ${PROJECT_SOURCE_DIR}/data/app.manifest MANIFEST_FILE)

add_library(glow_glow_compile_features INTERFACE)
add_library(glow::glow_compile_features ALIAS glow_glow_compile_features)
target_compile_features(
    glow_glow_compile_features
    INTERFACE
    c_std_17
    cxx_std_23
)

add_library(glow_glow_compile_options INTERFACE)
add_library(glow::glow_compile_options ALIAS glow_glow_compile_options)
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(
        glow_glow_compile_options
        INTERFACE
        /bigobj
        /diagnostics:caret
        /W4
        /WX
        /wd4100
        /Zc:__cplusplus
    )
    target_link_options(
        glow_glow_compile_options
        INTERFACE
        /WX
    )
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(
        glow_glow_compile_options
        INTERFACE
        -Wall
        -Wextra
        -Wpedantic
        -Wno-unused-parameters
        -Werror
    )
    target_link_options(
        glow_glow_compile_options
        INTERFACE
        --fatal-warnings
    )
endif()

add_library(glow_glow_include_directories INTERFACE)
add_library(glow::glow_include_directories ALIAS glow_glow_include_directories)
target_include_directories(
    glow_glow_include_directories
    INTERFACE
    ${PROJECT_SOURCE_DIR}/Glow
    ${PROJECT_BINARY_DIR}/include
)
target_compile_definitions(
    glow_glow_include_directories
    INTERFACE
    NLOHMANN_JSON_NAMESPACE_NO_VERSION=1
)

add_library(glow_glow_link_libraries INTERFACE)
add_library(glow::glow_link_libraries ALIAS glow_glow_link_libraries)
target_link_directories(
    glow_glow_link_libraries
    INTERFACE
    ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
)
target_link_libraries(
    glow_glow_link_libraries
    INTERFACE
    nlohmann_json::nlohmann_json
    tomlplusplus::tomlplusplus
    sqlite::sqlite
    dwmapi.lib
    Comctl32.lib
    gdiplus.lib
    CoreMessaging.lib
    WebView2LoaderStatic.lib
)

add_subdirectory(Glow)

if(GLOW_BUILD_ICONCONVERTER)
    add_subdirectory(${PROJECT_SOURCE_DIR}/tools/IconConverter)
endif()

if(GLOW_BUILD_EXAMPLES)
    add_subdirectory(${PROJECT_SOURCE_DIR}/examples)
endif()
