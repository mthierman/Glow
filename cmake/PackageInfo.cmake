set(NUGET_PATH "${CMAKE_BINARY_DIR}/_deps/nuget")
set(WEBVIEW "Microsoft.Web.WebView2")
set(WEBVIEW_VER "1.0.2151.40")
set(CPPWINRT "Microsoft.Windows.CppWinRT")
set(CPPWINRT_VER "2.0.230706.1")
set(WIL "Microsoft.Windows.ImplementationLibrary")
set(WIL_VER "1.0.231028.1")
set(
    URL_WIL
    "https://github.com/microsoft/wil/archive/refs/tags/v1.0.231028.1.tar.gz"
)
set(
    URL_JSON
    "https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz"
)
set(
    URL_TOML
    "https://github.com/marzer/tomlplusplus/archive/refs/tags/v3.4.0.tar.gz"
)
set(
    URL_SQLITE
    "https://www.sqlite.org/2023/sqlite-amalgamation-3440200.zip"
)
set(
    URL_CURL
    "https://github.com/curl/curl/releases/download/curl-8_4_0/curl-8.4.0.tar.xz"
)

configure_file(
    ${PROJECT_SOURCE_DIR}/config/GlowConfig.h.in
    ${CMAKE_INCLUDE_OUTPUT_DIRECTORY}/GlowConfig.h
)
