include(FetchContent)

FetchContent_Declare(
    cmake-modules URL "https://github.com/mthierman/cmake-modules/archive/refs/heads/main.zip"
    DOWNLOAD_NO_PROGRESS TRUE
    )

FetchContent_MakeAvailable(cmake-modules)

fetch_wil(
    VERSION
    1.0.240803.1
    )

fetch_cppwinrt(
    VERSION
    2.0.240405.15
    )

fetch_webview2(
    VERSION
    1.0.2739.15
    )
