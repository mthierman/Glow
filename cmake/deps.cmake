include(FetchContent)

FetchContent_Declare(
    cmake-modules
    GIT_REPOSITORY "https://github.com/mthierman/cmake-modules.git"
    GIT_TAG main
    GIT_SHALLOW ON
    )

FetchContent_MakeAvailable(cmake-modules)

include("${cmake-modules_SOURCE_DIR}/cmake-modules.cmake")

fetch_common()

fetch_wil(
    VERSION
    1.0.240122.1
    )

fetch_cppwinrt(
    VERSION
    2.0.240405.15
    )

fetch_webview2(
    VERSION
    1.0.2592.51
    )

fetch_ada(
    VERSION
    2.8.0
    )

fetch_json(
    VERSION
    3.11.3
    )

fetch_sqlite(
    VERSION
    3460000
    )
