cmake_minimum_required(VERSION 3.26)

include(FetchContent)

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

option(GLOW_BUILD_EXAMPLES OFF)
option(GLOW_BUILD_SHARED_LIBS OFF)

set(WEBVIEW "Microsoft.Web.WebView2")
set(WEBVIEW_VER "1.0.2151.40")
set(CPPWINRT "Microsoft.Windows.CppWinRT")
set(CPPWINRT_VER "2.0.230706.1")

execute_process(
    COMMAND
    nuget install ${WEBVIEW} -Version ${WEBVIEW_VER} -OutputDirectory ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Nuget
)
file(TO_CMAKE_PATH ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Nuget/${WEBVIEW}.${WEBVIEW_VER} webview_SOURCE_DIR)

execute_process(
    COMMAND
    nuget install ${CPPWINRT} -Version ${CPPWINRT_VER} -OutputDirectory ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Nuget
)
file(TO_CMAKE_PATH ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Nuget/${CPPWINRT}.${CPPWINRT_VER} cppwinrt_SOURCE_DIR)

FetchContent_Declare(
    wil
    URL https://github.com/microsoft/wil/archive/refs/tags/v1.0.231028.1.zip
)
FetchContent_Populate(wil)

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
    COMMAND cppwinrt -input ${webview_SOURCE_DIR}/lib/Microsoft.Web.WebView2.Core.winmd sdk -output ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    WORKING_DIRECTORY ${cppwinrt_SOURCE_DIR}/bin
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

add_library(glow_glow_includes INTERFACE)
add_library(glow::glow_includes ALIAS glow_glow_includes)
target_include_directories(
    glow_glow_includes
    INTERFACE
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_BINARY_DIR}
    ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    ${webview_SOURCE_DIR}/build/native/include
    ${wil_SOURCE_DIR}/include
    ${sqlite_SOURCE_DIR}
)
target_compile_definitions(
    glow_glow_includes
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
    glow::glow_includes
    glow::glow_libs
)

configure_file(${PROJECT_SOURCE_DIR}/config/GlowConfig.h.in ${PROJECT_BINARY_DIR}/GlowConfig.h)
file(TO_CMAKE_PATH ${PROJECT_SOURCE_DIR}/data/app.manifest MANIFEST_FILE)

add_executable(
    IconConverter
    ${PROJECT_SOURCE_DIR}/tools/IconConverter.cxx
    ${MANIFEST_FILE}
)

target_link_libraries(
    IconConverter
    PRIVATE
    glow::glow_compile_features
    glow::glow_compile_options
    glow::glow_includes
    windowscodecs.lib
)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_link_options(
        IconConverter
        PRIVATE
        /ILK:${CMAKE_PDB_OUTPUT_DIRECTORY}/IconConverter.ilk
    )
endif()

if(GLOW_BUILD_EXAMPLES)
    file(GENERATE OUTPUT ${PROJECT_BINARY_DIR}/res.rc CONTENT "1 ICON \"$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.ico\"")
    file(TO_CMAKE_PATH ${PROJECT_BINARY_DIR}/res.rc RC_FILE)
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

    add_library(glow_glow_examples INTERFACE)
    add_library(glow::glow_examples ALIAS glow_glow_examples)
    target_link_libraries(
        glow_glow_examples
        INTERFACE
        glow::glow_compile_features
        glow::glow_compile_options
        glow::glow_includes
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

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_link_options(
        Console
        PRIVATE
        /ILK:${CMAKE_PDB_OUTPUT_DIRECTORY}/Console.ilk
    )

    target_link_options(
        Win32
        PRIVATE
        /ILK:${CMAKE_PDB_OUTPUT_DIRECTORY}/Win32.ilk
    )

    target_link_options(
        WebView
        PRIVATE
        /ILK:${CMAKE_PDB_OUTPUT_DIRECTORY}/WebView.ilk
    )

    target_link_options(
        WebViewComposition
        PRIVATE
        /ILK:${CMAKE_PDB_OUTPUT_DIRECTORY}/WebViewComposition.ilk
    )
endif()

    target_link_libraries(Console PRIVATE glow::glow_examples)
    target_link_libraries(Win32 PRIVATE glow::glow_examples)
    target_link_libraries(WebView PRIVATE glow::glow_examples)
    target_link_libraries(WebViewComposition PRIVATE glow::glow_examples)

    add_dependencies(Console MakeIcon)
    add_dependencies(Win32 MakeIcon)
    add_dependencies(WebView MakeIcon)
    add_dependencies(WebViewComposition MakeIcon)
endif()
