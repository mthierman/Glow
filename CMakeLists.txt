cmake_minimum_required(VERSION 3.26)
set(LIB_NAME "Glow")
set(LIB_VERSION "0.0.1")
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${LIB_NAME}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${LIB_NAME}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${LIB_NAME}")
set(CMAKE_PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/PDB")

include(FetchContent)

project(
    ${LIB_NAME}
    VERSION ${LIB_VERSION}
    LANGUAGES C CXX ASM
)

file(WRITE "${CMAKE_BINARY_DIR}/version.txt" "v${LIB_VERSION}")
execute_process(
    COMMAND
    git --no-pager log -5 --oneline --no-decorate
    OUTPUT_FILE "${CMAKE_BINARY_DIR}/release_notes.txt"
)

set(WEBVIEW "Microsoft.Web.WebView2")
set(WEBVIEW_VER "1.0.2151.40")
set(CPPWINRT "Microsoft.Windows.CppWinRT")
set(CPPWINRT_VER "2.0.230706.1")
set(WIL "Microsoft.Windows.ImplementationLibrary")
set(WIL_VER "1.0.230824.2")

execute_process(
    COMMAND
    nuget install "${WEBVIEW}" -Version "${WEBVIEW_VER}" -OutputDirectory "${CMAKE_BINARY_DIR}/Nuget"
)
file(TO_CMAKE_PATH "${CMAKE_BINARY_DIR}/Nuget/${WEBVIEW}.${WEBVIEW_VER}" webview_SOURCE_DIR)

execute_process(
    COMMAND
    nuget install "${CPPWINRT}" -Version "${CPPWINRT_VER}" -OutputDirectory "${CMAKE_BINARY_DIR}/Nuget"
)
file(TO_CMAKE_PATH "${CMAKE_BINARY_DIR}/Nuget/${CPPWINRT}.${CPPWINRT_VER}" cppwinrt_SOURCE_DIR)

execute_process(
    COMMAND
    nuget install "${WIL}" -Version "${WIL_VER}" -OutputDirectory "${CMAKE_BINARY_DIR}/Nuget"
)
file(TO_CMAKE_PATH "${CMAKE_BINARY_DIR}/Nuget/${WIL}.${WIL_VER}" wil_SOURCE_DIR)

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

add_library(sqlite "${sqlite_SOURCE_DIR}/sqlite3.c" "${sqlite_SOURCE_DIR}/sqlite3.h")
set_target_properties(sqlite PROPERTIES LINKER_LANGUAGE C)

execute_process(
    COMMAND cppwinrt -input Microsoft.Web.WebView2.Core.winmd sdk WORKING_DIRECTORY "${webview_SOURCE_DIR}/lib"
)
file(COPY "${webview_SOURCE_DIR}/runtimes/win-x64/native_uap/Microsoft.Web.WebView2.Core.dll" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

add_library(
    common
    INTERFACE
)

target_compile_definitions(
    common
    INTERFACE
    LIB_NAME="${LIB_NAME}"
    LIB_VERSION="${LIB_VERSION}"
    NLOHMANN_JSON_NAMESPACE_NO_VERSION=1
)

target_include_directories(
    common
    INTERFACE
    ${sqlite_SOURCE_DIR}
    ${webview_SOURCE_DIR}/lib
    ${webview_SOURCE_DIR}/build/native/include
)

target_link_libraries(
    common
    INTERFACE
    dwmapi.lib
    Comctl32.lib
    gdiplus.lib
    gdiplus.lib
    nlohmann_json::nlohmann_json
    tomlplusplus::tomlplusplus
    sqlite
)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(
        common
        INTERFACE
        /await:strict
        /bigobj
        /diagnostics:caret
        /EHsc
        /permissive-
        /utf-8
        /W4
        /WX
        /Zc:__cplusplus
    )

    target_link_options(
        common
        INTERFACE
        /WX
        /INCREMENTAL:NO
    )
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(
        common
        INTERFACE
        -fcoroutines
    )
endif()

file(TO_CMAKE_PATH "${CMAKE_SOURCE_DIR}/data/app.manifest" MANIFEST_FILE)

# IconConverter
file(GENERATE OUTPUT "${CMAKE_BINARY_DIR}/res.rc" CONTENT "1 ICON \"$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.ico\"")
file(TO_CMAKE_PATH "${CMAKE_BINARY_DIR}/res.rc" RC_FILE)
set(PNG_FILE ${CMAKE_SOURCE_DIR}/data/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.png)
set(ICO_FILE ${CMAKE_BINARY_DIR}/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.ico)

add_executable(
    IconConverter
    ${CMAKE_SOURCE_DIR}/tools/IconConverter.cxx
    ${MANIFEST_FILE}
)

set_property(
    TARGET IconConverter
    APPEND PROPERTY LINK_LIBRARIES common
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

# Glow
file(GLOB HEADERS_CONSOLE CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/console/*.hxx")
file(GLOB HEADERS_FILESYSTEM CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/filesystem/*.hxx")
file(GLOB HEADERS_GUI CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/gui/*.hxx")
file(GLOB HEADERS_SETTINGS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/settings/*.hxx")
file(GLOB HEADERS_TEXT CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/text/*.hxx")

add_library(
    ${LIB_NAME}
    STATIC
    ${CMAKE_SOURCE_DIR}/src/Glow.cxx
    ${HEADERS_CONSOLE}
    ${HEADERS_FILESYSTEM}
    ${HEADERS_GUI}
    ${HEADERS_SETTINGS}
    ${HEADERS_TEXT}
)

set_property(
    TARGET ${LIB_NAME}
    APPEND PROPERTY LINK_LIBRARIES common
)

# App
add_executable(
    App
    WIN32
    ${CMAKE_SOURCE_DIR}/tests/App.cxx
    ${RC_FILE}
    ${MANIFEST_FILE}
)

set_property(
    TARGET App
    APPEND PROPERTY LINK_LIBRARIES common
)

# Console
add_executable(
    Console
    ${CMAKE_SOURCE_DIR}/tests/Console.cxx
    ${RC_FILE}
    ${MANIFEST_FILE}
)

set_property(
    TARGET Console
    APPEND PROPERTY LINK_LIBRARIES common
)

# Settings
add_executable(
    Settings
    ${CMAKE_SOURCE_DIR}/tests/Settings.cxx
    ${RC_FILE}
    ${MANIFEST_FILE}
)

set_property(
    TARGET Settings
    APPEND PROPERTY LINK_LIBRARIES common
)

# WebView
add_executable(
    WebView
    WIN32
    ${CMAKE_SOURCE_DIR}/tests/WebView.cxx
    ${RC_FILE}
    ${MANIFEST_FILE}
)

set_property(
    TARGET WebView
    APPEND PROPERTY LINK_LIBRARIES common
)

target_link_directories(
    WebView
    PRIVATE
    ${webview_SOURCE_DIR}/build/native/x64
)

target_link_libraries(
    WebView
    PRIVATE
    WebView2LoaderStatic.lib
)

# WebViewComposition
add_executable(
    WebViewComposition
    WIN32
    ${CMAKE_SOURCE_DIR}/tests/WebViewComposition.cxx
    ${RC_FILE}
    ${MANIFEST_FILE}
)

set_property(
    TARGET WebViewComposition
    APPEND PROPERTY LINK_LIBRARIES common
)

target_link_libraries(
    WebViewComposition
    PRIVATE
    WindowsApp.lib
)

add_dependencies(App MakeIcon)
add_dependencies(Console MakeIcon)
add_dependencies(Settings MakeIcon)
add_dependencies(WebView MakeIcon)
add_dependencies(WebViewComposition MakeIcon)
