add_library(Glow)

add_library(
    glow::glow
    ALIAS
    Glow
)

configure_file(
    ${PROJECT_SOURCE_DIR}/config.hxx
    ${CMAKE_CURRENT_BINARY_DIR}/config/glow/config.hxx
)

target_sources(
    Glow
    PRIVATE # glow/console.cxx # glow/filesystem.cxx glow/gui.cxx glow/text.cxx
            glow/helpers.cxx
    PUBLIC FILE_SET
           HEADERS
           BASE_DIRS
           ${PROJECT_SOURCE_DIR}/Glow
           ${CMAKE_CURRENT_BINARY_DIR}/config
           FILES
           ${CMAKE_CURRENT_BINARY_DIR}/config/glow/config.hxx
           glow/helpers.hxx
           glow/test.hxx
)

target_include_directories(
    Glow
    PUBLIC ${PROJECT_SOURCE_DIR}/Glow
           ${webview2_SOURCE_DIR}/build/native/include-winrt
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
           Uxtheme.lib
           Comctl32.lib
           gdiplus.lib
           windowscodecs.lib
           ShLwApi.Lib
           WebView2LoaderStatic.lib
)
