add_library(${PROJECT_NAME})

add_library(
    glow::glow
    ALIAS
    ${PROJECT_NAME}
)

configure_file(
    config/glow.hxx.in
    ${PROJECT_BINARY_DIR}/include/config/glow.hxx
)

target_sources(
    ${PROJECT_NAME}
    PRIVATE console/console.cxx
            filesystem/database.cxx
            filesystem/filesystem.cxx
            gui/app.cxx
            gui/gui.cxx
            gui/webview.cxx
            # gui/webview_composition.cxx
            logging/logging.cxx
            text/text.cxx
            data/glow.manifest
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
           gui/app.hxx
           gui/gui.hxx
           # gui/webview_composition.hxx
           gui/webview.hxx
           logging/logging.hxx
           text/text.hxx
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC ${CMAKE_BINARY_DIR}/_deps/Nuget/${WEBVIEW}.${WEBVIEW_VER}/build/native/include-winrt
           ${CMAKE_BINARY_DIR}/_deps/Nuget/${WEBVIEW}.${WEBVIEW_VER}/build/native/include
           ${CMAKE_BINARY_DIR}/_deps/Nuget/${WIL}.${WIL_VER}/include
           ${sqlite_SOURCE_DIR}
           ${json_SOURCE_DIR}/single_include
           ${toml_SOURCE_DIR}/include
)

target_link_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${CMAKE_BINARY_DIR}/_deps/Nuget/${WEBVIEW}.${WEBVIEW_VER}/build/native/x64
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE glow::flags
    PUBLIC glow::features
           nlohmann_json::nlohmann_json
           tomlplusplus::tomlplusplus
           sqlite::sqlite
           dwmapi.lib
           Comctl32.lib
           gdiplus.lib
           WebView2LoaderStatic.lib
)

target_compile_definitions(
    ${PROJECT_NAME}
    PUBLIC NLOHMANN_JSON_NAMESPACE_NO_VERSION=1
)
