add_library(Glow)

add_library(
    glow::glow
    ALIAS
    Glow
)

configure_file(
    ${PROJECT_SOURCE_DIR}/config/glow.hxx.in
    ${PROJECT_BINARY_DIR}/include/config/glow.hxx
)

target_sources(
    Glow
    PRIVATE console/console.cxx
            filesystem/database.cxx
            filesystem/filesystem.cxx
            gui/gui.cxx
            gui/webview.cxx
            gui/window.cxx
            logging/logging.cxx
            text/text.cxx
    PUBLIC FILE_SET
           HEADERS
           BASE_DIRS
           ${CMAKE_CURRENT_SOURCE_DIR}
           ${PROJECT_BINARY_DIR}/include
           FILES
           ${PROJECT_BINARY_DIR}/include/config/glow.hxx
           console/console.hxx
           filesystem/database.hxx
           filesystem/filesystem.hxx
           gui/gui.hxx
           gui/webview.hxx
           gui/window.hxx
           logging/logging.hxx
           text/text.hxx
)

target_include_directories(
    Glow
    PUBLIC ${webview2_SOURCE_DIR}/build/native/include-winrt
           ${webview2_SOURCE_DIR}/build/native/include
           ${wil_SOURCE_DIR}/include
           ${sqlite_SOURCE_DIR}
           ${json_SOURCE_DIR}/single_include
           ${toml_SOURCE_DIR}/include
)

target_link_directories(
    Glow
    PUBLIC
    ${webview2_SOURCE_DIR}/build/native/x64
)

target_link_libraries(
    Glow
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
    Glow
    PUBLIC NLOHMANN_JSON_NAMESPACE_NO_VERSION=1
)
