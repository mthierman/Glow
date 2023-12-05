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
    INTERFACE ${GLOW_CONFIG}
              ${webview_SOURCE_DIR}/build/native/include-winrt
              ${webview_SOURCE_DIR}/build/native/include
              ${wil_SOURCE_DIR}/include
)

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
    ${webview_SOURCE_DIR}/build/native/x64
)
target_link_libraries(
    glow_glow_link_libraries
    INTERFACE nlohmann_json::nlohmann_json
              tomlplusplus::tomlplusplus
              sqlite::sqlite
              dwmapi.lib
              Comctl32.lib
              gdiplus.lib
              WebView2LoaderStatic.lib
)
target_compile_definitions(
    glow_glow_link_libraries
    INTERFACE NLOHMANN_JSON_NAMESPACE_NO_VERSION=1
)
