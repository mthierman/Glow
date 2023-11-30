cmake_minimum_required(VERSION 3.26)

set(LIB_NAME "Glow")
set(LIB_VERSION "0.0.1")

project(
    ${LIB_NAME}
    VERSION ${LIB_VERSION}
    LANGUAGES C CXX
)

option(GLOW_BUILD_EXAMPLES OFF)
option(GLOW_BUILD_ICONCONVERTER OFF)

# if (PROJECT_IS_TOP_LEVEL)
#     set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/libs)
#     set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/libs)
#     set(CMAKE_PDB_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/pdb)
#     set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${LIB_NAME})
# endif()

include(cmake/packages.cmake)

configure_file(${PROJECT_SOURCE_DIR}/config/GlowConfig.h.in ${PROJECT_BINARY_DIR}/include/GlowConfig.h)
file(TO_CMAKE_PATH ${PROJECT_SOURCE_DIR}/data/app.manifest MANIFEST_FILE)

# compile_features
add_library(glow_glow_compile_features INTERFACE)
add_library(glow::glow_compile_features ALIAS glow_glow_compile_features)
target_compile_features(
    glow_glow_compile_features
    INTERFACE
    c_std_17
    cxx_std_23
)

# compile_options
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

# include_directories
add_library(glow_glow_include_directories INTERFACE)
add_library(glow::glow_include_directories ALIAS glow_glow_include_directories)
target_include_directories(
    glow_glow_include_directories
    INTERFACE
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_BINARY_DIR}/include
    ${webview_SOURCE_DIR}/build/native/include
    ${wil_SOURCE_DIR}/include
    ${sqlite_SOURCE_DIR}
)
target_compile_definitions(
    glow_glow_include_directories
    INTERFACE
    NLOHMANN_JSON_NAMESPACE_NO_VERSION=1
)

# link_libraries
add_library(glow_glow_link_libraries INTERFACE)
add_library(glow::glow_link_libraries ALIAS glow_glow_link_libraries)
target_link_directories(
    glow_glow_link_libraries
    INTERFACE
    ${webview_SOURCE_DIR}/build/native/x64
)
target_link_libraries(
    glow_glow_link_libraries
    INTERFACE
    nlohmann_json::nlohmann_json
    tomlplusplus::tomlplusplus
    ${SQLITE_NAME}
    dwmapi.lib
    Comctl32.lib
    gdiplus.lib
    CoreMessaging.lib
    WebView2LoaderStatic.lib
)

# glow
set(
    GLOW_SOURCES
    ${PROJECT_SOURCE_DIR}/src/Glow.cxx
    ${PROJECT_SOURCE_DIR}/console/console.hxx
    ${PROJECT_SOURCE_DIR}/filesystem/filesystem.hxx
    ${PROJECT_SOURCE_DIR}/gui/app.hxx
    ${PROJECT_SOURCE_DIR}/gui/gui.hxx
    ${PROJECT_SOURCE_DIR}/gui/webview_composition.hxx
    ${PROJECT_SOURCE_DIR}/gui/webview.hxx
    ${PROJECT_SOURCE_DIR}/logging/logging.hxx
    ${PROJECT_SOURCE_DIR}/text/text.hxx
)
add_library(
    ${LIB_NAME}
    ${GLOW_SOURCES}
)
# add_library(glow::glow ALIAS ${LIB_NAME})
target_link_libraries(
    ${LIB_NAME}
    PRIVATE
    glow::glow_compile_features
    glow::glow_compile_options
    glow::glow_include_directories
    glow::glow_link_libraries
)
set_target_properties(
    ${LIB_NAME}
    PROPERTIES
    OUTPUT_NAME ${LIB_NAME}
)

if(GLOW_BUILD_ICONCONVERTER)
    add_executable(
        IconConverter
        ${PROJECT_SOURCE_DIR}/tools/IconConverter.cxx
        ${PROJECT_SOURCE_DIR}/console/console.hxx
        ${PROJECT_SOURCE_DIR}/filesystem/filesystem.hxx
        ${PROJECT_SOURCE_DIR}/text/text.hxx
        ${MANIFEST_FILE}
    )
    target_link_libraries(
        IconConverter
        PRIVATE
        glow::glow_compile_features
        glow::glow_compile_options
        glow::glow_include_directories
        windowscodecs.lib
    )
endif()

if(GLOW_BUILD_EXAMPLES)
    file(GENERATE OUTPUT ${PROJECT_BINARY_DIR}/res.rc CONTENT "1 ICON \"$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.ico\"")
    file(TO_CMAKE_PATH ${PROJECT_BINARY_DIR}/res.rc RC_FILE)

    if(GLOW_BUILD_ICONCONVERTER)
        set(PNG_FILE ${PROJECT_SOURCE_DIR}/data/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.png)
        set(ICO_FILE ${PROJECT_BINARY_DIR}/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.ico)
        add_custom_command(
            OUTPUT ${ICO_FILE}
            COMMAND IconConverter ${PNG_FILE} ${ICO_FILE}
            COMMENT "Running IconConverter..."
        )
        add_custom_target(
            MakeIcon ALL
            DEPENDS ${ICO_FILE}
        )
    else()
        set(ICO_SOURCE_FILE ${PROJECT_SOURCE_DIR}/data/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.ico)
        set(ICO_FILE ${PROJECT_BINARY_DIR}/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.ico)
        add_custom_command(
            OUTPUT ${ICO_FILE}
            COMMAND ${CMAKE_COMMAND} -E copy ${ICO_SOURCE_FILE} ${PROJECT_BINARY_DIR}
            COMMENT "Copying icon..."
        )
        add_custom_target(
            MakeIcon ALL
            DEPENDS ${ICO_FILE}
        )
    endif()

    add_library(glow_glow_examples INTERFACE)
    add_library(glow::glow_examples ALIAS glow_glow_examples)
    target_link_libraries(
        glow_glow_examples
        INTERFACE
        glow::glow_compile_features
        glow::glow_compile_options
        glow::glow_include_directories
        glow::glow_link_libraries
    )

    add_executable(
        App
        WIN32
        ${PROJECT_SOURCE_DIR}/tests/App.cxx
        ${PROJECT_SOURCE_DIR}/console/console.hxx
        ${PROJECT_SOURCE_DIR}/gui/app.hxx
        ${RC_FILE}
        ${MANIFEST_FILE}
    )
    target_link_libraries(App PRIVATE glow::glow_examples)

    add_executable(
        Console
        ${PROJECT_SOURCE_DIR}/tests/Console.cxx
        ${PROJECT_SOURCE_DIR}/filesystem/filesystem.hxx
        ${PROJECT_SOURCE_DIR}/logging/logging.hxx
        ${PROJECT_SOURCE_DIR}/text/text.hxx
        ${RC_FILE}
        ${MANIFEST_FILE}
    )
    target_link_libraries(Console PRIVATE glow::glow_examples)

    add_executable(
        WebView
        WIN32
        ${PROJECT_SOURCE_DIR}/tests/WebView.cxx
        ${PROJECT_SOURCE_DIR}/console/console.hxx
        ${PROJECT_SOURCE_DIR}/gui/app.hxx
        ${PROJECT_SOURCE_DIR}/gui/webview.hxx
        ${PROJECT_SOURCE_DIR}/tests/include/WebView.hxx
        ${RC_FILE}
        ${MANIFEST_FILE}
    )
    target_link_libraries(WebView PRIVATE glow::glow_examples)


    add_executable(
        WebViewComposition
        WIN32
        ${PROJECT_SOURCE_DIR}/tests/WebViewComposition.cxx
        ${PROJECT_SOURCE_DIR}/console/console.hxx
        ${PROJECT_SOURCE_DIR}/gui/app.hxx
        ${PROJECT_SOURCE_DIR}/gui/webview_composition.hxx
        ${PROJECT_SOURCE_DIR}/tests/include/WebView.hxx
        ${RC_FILE}
        ${MANIFEST_FILE}
    )
    target_link_libraries(WebViewComposition PRIVATE glow::glow_examples)

    add_dependencies(App MakeIcon)
    add_dependencies(Console MakeIcon)
    add_dependencies(WebView MakeIcon)
    add_dependencies(WebViewComposition MakeIcon)
endif()
