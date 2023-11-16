cmake_minimum_required(VERSION 3.26)

set(LIB_NAME "Glow")
set(LIB_VERSION "0.0.1")

project(
    ${LIB_NAME}
    VERSION ${LIB_VERSION}
    LANGUAGES C CXX
)

if (PROJECT_IS_TOP_LEVEL)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/libs)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/libs)
    set(CMAKE_PDB_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/pdb)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${LIB_NAME})
endif()

option(BUILD_EXAMPLES OFF)
option(BUILD_SHARED_LIBS OFF)

set(WEBVIEW "Microsoft.Web.WebView2")
set(WEBVIEW_VER "1.0.2151.40")
set(CPPWINRT "Microsoft.Windows.CppWinRT")
set(CPPWINRT_VER "2.0.230706.1")
set(WIL "Microsoft.Windows.ImplementationLibrary")
set(WIL_VER "1.0.230824.2")

include(FetchContent)

file(WRITE ${PROJECT_BINARY_DIR}/version.txt v${PROJECT_VERSION})
execute_process(
    COMMAND git --no-pager log -5 --oneline --no-decorate
    OUTPUT_FILE ${PROJECT_BINARY_DIR}/release_notes.txt
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

configure_file(${PROJECT_SOURCE_DIR}/config/GlowConfig.h.in ${PROJECT_BINARY_DIR}/GlowConfig.h)
file(TO_CMAKE_PATH ${PROJECT_SOURCE_DIR}/data/app.manifest MANIFEST_FILE)

execute_process(
    COMMAND
    nuget install ${WEBVIEW} -Version ${WEBVIEW_VER} -OutputDirectory ${CMAKE_BINARY_DIR}
)
file(TO_CMAKE_PATH ${CMAKE_BINARY_DIR}/${WEBVIEW}.${WEBVIEW_VER} webview_SOURCE_DIR)

execute_process(
    COMMAND
    nuget install ${CPPWINRT} -Version ${CPPWINRT_VER} -OutputDirectory ${CMAKE_BINARY_DIR}
)
file(TO_CMAKE_PATH ${CMAKE_BINARY_DIR}/${CPPWINRT}.${CPPWINRT_VER} cppwinrt_SOURCE_DIR)

execute_process(
    COMMAND
    nuget install ${WIL} -Version ${WIL_VER} -OutputDirectory ${CMAKE_BINARY_DIR}
)
file(TO_CMAKE_PATH ${CMAKE_BINARY_DIR}/${WIL}.${WIL_VER} wil_SOURCE_DIR)

FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
    toml
    URL https://github.com/marzer/tomlplusplus/archive/refs/tags/v3.4.0.zip
)
FetchContent_MakeAvailable(toml)

FetchContent_Declare(
    sqlite
    URL https://www.sqlite.org/2023/sqlite-amalgamation-3440000.zip
)
FetchContent_MakeAvailable(sqlite)

# FetchContent_Declare(
#     curl
#     URL https://github.com/curl/curl/releases/download/curl-8_4_0/curl-8.4.0.tar.xz
# )
# FetchContent_MakeAvailable(curl)

execute_process(
    COMMAND cppwinrt -input Microsoft.Web.WebView2.Core.winmd sdk
    WORKING_DIRECTORY ${webview_SOURCE_DIR}/lib
)
file(COPY ${webview_SOURCE_DIR}/runtimes/win-x64/native_uap/Microsoft.Web.WebView2.Core.dll DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_library(sqlite ${sqlite_SOURCE_DIR}/sqlite3.c ${sqlite_SOURCE_DIR}/sqlite3.h)
set_target_properties(sqlite PROPERTIES LINKER_LANGUAGE C)

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
        /Zc:__cplusplus
    )

    target_link_options(
        glow_glow_compile_options
        INTERFACE
        /WX
    )
endif()

add_library(glow_glow_settings INTERFACE)
add_library(glow::glow_settings ALIAS glow_glow_settings)
target_include_directories(
    glow_glow_settings
    INTERFACE
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_BINARY_DIR}
    ${webview_SOURCE_DIR}/lib
    ${webview_SOURCE_DIR}/build/native/include
    ${cppwinrt_SOURCE_DIR}
    ${wil_SOURCE_DIR}
    ${sqlite_SOURCE_DIR}
)
target_compile_definitions(
    glow_glow_settings
    INTERFACE
    NLOHMANN_JSON_NAMESPACE_NO_VERSION=1
)

add_library(glow_glow_libs INTERFACE)
add_library(glow::glow_libs ALIAS glow_glow_libs)
target_link_directories(
    glow_glow_libs
    INTERFACE
    ${webview_SOURCE_DIR}/build/native/x64
)
target_link_libraries(
    glow_glow_libs
    INTERFACE
    nlohmann_json::nlohmann_json
    tomlplusplus::tomlplusplus
    sqlite
    dwmapi.lib
    Comctl32.lib
    gdiplus.lib
    CoreMessaging.lib
    WebView2LoaderStatic.lib
)

add_library(glow_glow ${PROJECT_SOURCE_DIR}/src/Glow.cxx)
add_library(glow::glow ALIAS glow_glow)
target_link_libraries(
    glow_glow
    PRIVATE
    glow::glow_compile_features
    glow::glow_compile_options
    glow::glow_settings
    glow::glow_libs
)

file(GENERATE OUTPUT "${CMAKE_BINARY_DIR}/res.rc" CONTENT "1 ICON \"$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.ico\"")
file(TO_CMAKE_PATH "${CMAKE_BINARY_DIR}/res.rc" RC_FILE)
set(PNG_FILE ${PROJECT_SOURCE_DIR}/data/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.png)
set(ICO_FILE ${CMAKE_BINARY_DIR}/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.ico)

add_executable(
    IconConverter
    ${PROJECT_SOURCE_DIR}/tools/IconConverter.cxx
    ${MANIFEST_FILE}
    ${PROJECT_SOURCE_DIR}/data/IconConverter.rc
)

target_link_libraries(
    IconConverter
    PRIVATE
    glow::glow_compile_features
    glow::glow_compile_options
    glow::glow_settings
    gdiplus.lib
)

add_custom_command(
    OUTPUT ${ICO_FILE}
    COMMAND IconConverter ${PNG_FILE} ${ICO_FILE}
    COMMENT "Running IconConverter..."
)

add_custom_target(
    MakeIcon ALL
    DEPENDS ${ICO_FILE}
)

if(BUILD_EXAMPLES)
    add_library(glow_glow_examples INTERFACE)
    add_library(glow::glow_examples ALIAS glow_glow_examples)
    target_link_libraries(
        glow_glow_examples
        INTERFACE
        glow::glow_compile_features
        glow::glow_compile_options
        glow::glow_settings
        glow::glow_libs
    )

    add_executable(
        Console
        ${PROJECT_SOURCE_DIR}/tests/Console.cxx
        ${RC_FILE}
        ${MANIFEST_FILE}
    )

    add_executable(
        Win32
        WIN32
        ${PROJECT_SOURCE_DIR}/tests/App.cxx
        ${RC_FILE}
        ${MANIFEST_FILE}
    )

    add_executable(
        WebView
        WIN32
        ${PROJECT_SOURCE_DIR}/tests/WebView.cxx
        ${RC_FILE}
        ${MANIFEST_FILE}
    )

    add_executable(
        WebViewComposition
        WIN32
        ${PROJECT_SOURCE_DIR}/tests/WebViewComposition.cxx
        ${RC_FILE}
        ${MANIFEST_FILE}
    )

    target_link_libraries(Console PRIVATE glow::glow_examples)
    target_link_libraries(Win32 PRIVATE glow::glow_examples)
    target_link_libraries(WebView PRIVATE glow::glow_examples)
    target_link_libraries(WebViewComposition PRIVATE glow::glow_examples)

    add_dependencies(Console MakeIcon)
    add_dependencies(Win32 MakeIcon)
    add_dependencies(WebView MakeIcon)
    add_dependencies(WebViewComposition MakeIcon)
endif()
