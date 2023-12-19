add_library(glow)

add_library(
    glow::glow
    ALIAS
    glow
)

configure_file(
    config/glow.hxx.in
    ${PROJECT_BINARY_DIR}/include/config/glow.hxx
)

target_sources(
    glow
    PRIVATE console/console.cxx
            filesystem/database.cxx
            filesystem/filesystem.cxx
            gui/app.cxx
            gui/gui.cxx
            # gui/webview_composition.cxx
            gui/webview_window.cxx
            gui/webview.cxx
            gui/window.cxx
            logging/logging.cxx
            text/text.cxx
    PUBLIC FILE_SET
           HEADERS
           FILES
           console/console.hxx
           filesystem/database.hxx
           filesystem/filesystem.hxx
           gui/app.hxx
           gui/gui.hxx
           # gui/webview_composition.hxx
           gui/webview_window.hxx
           gui/webview.hxx
           gui/window.hxx
           logging/logging.hxx
           text/text.hxx
)

target_include_directories(
    glow
    PUBLIC ${PROJECT_BINARY_DIR}/include
           ${CMAKE_BINARY_DIR}/_deps/Nuget/${WEBVIEW2}.${WEBVIEW2_VER}/build/native/include-winrt
           ${CMAKE_BINARY_DIR}/_deps/Nuget/${WEBVIEW2}.${WEBVIEW2_VER}/build/native/include
           ${CMAKE_BINARY_DIR}/_deps/Nuget/${WIL}.${WIL_VER}/include
           ${sqlite_SOURCE_DIR}
           ${json_SOURCE_DIR}/single_include
           ${toml_SOURCE_DIR}/include
)

target_link_directories(
    glow
    PUBLIC
    ${CMAKE_BINARY_DIR}/_deps/Nuget/${WEBVIEW2}.${WEBVIEW2_VER}/build/native/x64
)

target_link_libraries(
    glow
    PRIVATE glow::flags
    PUBLIC glow::features
           glow::definitions
           nlohmann_json::nlohmann_json
           tomlplusplus::tomlplusplus
           sqlite::sqlite
           dwmapi.lib
           Comctl32.lib
           gdiplus.lib
           WebView2LoaderStatic.lib
)

target_compile_definitions(
    glow
    PUBLIC NLOHMANN_JSON_NAMESPACE_NO_VERSION=1
)
