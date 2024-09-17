FetchContent_Declare(
    microsoft_webview2
    URL "https://www.nuget.org/api/v2/package/Microsoft.Web.WebView2/1.0.2739.15"
    DOWNLOAD_NO_PROGRESS TRUE
    SOURCE_SUBDIR
    "NULL"
    )

FetchContent_MakeAvailable(microsoft_webview2)

add_library(
    microsoft_webview2
    INTERFACE
    )

add_library(
    microsoft::webview2
    ALIAS
    microsoft_webview2
    )

target_sources(
    microsoft_webview2
    INTERFACE FILE_SET
              HEADERS
              BASE_DIRS
              "${microsoft_webview2_SOURCE_DIR}/build/native/include"
              "${microsoft_webview2_SOURCE_DIR}/build/native/include-winrt"
    )

target_link_directories(
    microsoft_webview2
    INTERFACE
    "${microsoft_webview2_SOURCE_DIR}/build/native/x64"
    )

target_link_libraries(microsoft_webview2 INTERFACE WebView2LoaderStatic.lib)
