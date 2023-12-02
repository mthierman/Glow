include(FetchContent)

# Nuget
include(deps/WIL)
include(deps/WebView)
include(deps/CPPWinRT)

# FetchContent
include(deps/JSON)
include(deps/TOML)
include(deps/SQLite)
# include(deps/CURL)

execute_process(
    COMMAND
        cppwinrt -input ${webview_SOURCE_DIR}/lib/Microsoft.Web.WebView2.Core.winmd sdk -output
        ${webview_SOURCE_DIR}/build/native/include-winrt
    WORKING_DIRECTORY ${cppwinrt_SOURCE_DIR}/bin
)
