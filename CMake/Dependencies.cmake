include(FetchContent)
include(PackageInfo)

# Nuget
include(deps/WIL)
include(deps/WebView)
include(deps/CPPWinRT)

execute_process(
    COMMAND
        cppwinrt -input ${webview_SOURCE_DIR}/lib/Microsoft.Web.WebView2.Core.winmd sdk -output
        ${webview_SOURCE_DIR}/build/native/include-winrt
    WORKING_DIRECTORY ${cppwinrt_SOURCE_DIR}/bin
)

# FetchContent
FetchContent_Declare(
    json
    URL ${URL_JSON}
)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
    toml
    URL ${URL_TOML}
)
FetchContent_MakeAvailable(toml)

include(Data)
include(Common)
