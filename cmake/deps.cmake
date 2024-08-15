include(FetchContent)

FetchContent_Declare(
    cmake-modules
    GIT_REPOSITORY "https://github.com/mthierman/cmake-modules.git"
    GIT_TAG main
    GIT_SHALLOW ON
    )

FetchContent_MakeAvailable(cmake-modules)

fetch_common()

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
    1.0.2651.64
    )
