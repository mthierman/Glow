include(SQLite)

# glow::glow_compile_features
add_library(
    glow_glow_compile_features
    INTERFACE
)

add_library(
    glow::glow_compile_features
    ALIAS
    glow_glow_compile_features
)

target_compile_features(
    glow_glow_compile_features
    INTERFACE c_std_17
              cxx_std_23
)

# glow::glow_compile_options
add_library(
    glow_glow_compile_options
    INTERFACE
)

add_library(
    glow::glow_compile_options
    ALIAS
    glow_glow_compile_options
)

if(CMAKE_CXX_COMPILER_ID
   MATCHES
   "MSVC"
)
    target_compile_options(
        glow_glow_compile_options
        INTERFACE /bigobj
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

# glow::glow_include_directories
add_library(
    glow_glow_include_directories
    INTERFACE
)

add_library(
    glow::glow_include_directories
    ALIAS
    glow_glow_include_directories
)

target_include_directories(
    glow_glow_include_directories
    INTERFACE ${CMAKE_BINARY_DIR}/config
              ${webview_SOURCE_DIR}/build/native/include-winrt
              ${webview_SOURCE_DIR}/build/native/include
              ${wil_SOURCE_DIR}/include
)

# glow::glow_link_libraries
add_library(
    glow_glow_link_libraries
    INTERFACE
)

add_library(
    glow::glow_link_libraries
    ALIAS
    glow_glow_link_libraries
)

target_link_directories(
    glow_glow_link_libraries
    INTERFACE
    ${NUGET_DIRECTORY}/${WEBVIEW}.${WEBVIEW_VER}/build/native/x64
)

target_link_libraries(
    glow_glow_link_libraries
    INTERFACE nlohmann_json::nlohmann_json
              tomlplusplus::tomlplusplus
              glow::sqlite
              dwmapi.lib
              Comctl32.lib
              gdiplus.lib
              WebView2LoaderStatic.lib
)

target_compile_definitions(
    glow_glow_link_libraries
    INTERFACE NLOHMANN_JSON_NAMESPACE_NO_VERSION=1
)

# glow::glow
add_library(Glow)

add_library(
    glow::glow
    ALIAS
    Glow
)

target_sources(
    Glow
    PRIVATE console/console.cxx
            filesystem/filesystem.cxx
            gui/app.cxx
            gui/gui.cxx
            gui/webview.cxx
            gui/webview_composition.cxx
            logging/logging.cxx
            text/text.cxx
)

target_sources(
    Glow
    PUBLIC FILE_SET
           HEADERS
           FILES
           console/console.hxx
           filesystem/filesystem.hxx
           gui/app.hxx
           gui/gui.hxx
           gui/webview_composition.hxx
           gui/webview.hxx
           logging/logging.hxx
           text/text.hxx
)

target_link_libraries(
    Glow
    PUBLIC glow::glow_compile_features
           glow::glow_compile_options
           glow::glow_include_directories
           glow::glow_link_libraries
)

# set_target_properties( Glow PROPERTIES OUTPUT_NAME Glow )
