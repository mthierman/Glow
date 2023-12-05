add_library(${PROJECT_NAME})

add_library(
    glow::glow
    ALIAS
    ${PROJECT_NAME}
)

target_sources(
    ${PROJECT_NAME}
    PRIVATE console/console.cxx
            filesystem/database.cxx
            filesystem/filesystem.cxx
            gui/app.cxx
            gui/gui.cxx
            gui/webview.cxx
            gui/webview_composition.cxx
            logging/logging.cxx
            text/text.cxx
)

target_sources(
    ${PROJECT_NAME}
    PUBLIC FILE_SET
           HEADERS
           FILES
           console/console.hxx
           filesystem/database.hxx
           filesystem/filesystem.hxx
           gui/app.hxx
           gui/gui.hxx
           gui/webview_composition.hxx
           gui/webview.hxx
           logging/logging.hxx
           text/text.hxx
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC ${GLOW_CONFIG}
           ${webview_SOURCE_DIR}/build/native/include-winrt
           ${webview_SOURCE_DIR}/build/native/include
           ${wil_SOURCE_DIR}/include
)

target_link_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${webview_SOURCE_DIR}/build/native/x64
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
