set(FETCHCONTENT_SOURCE_DIR_CURL
    "$ENV{LIBRARIES}/curl"
)

set(FETCHCONTENT_SOURCE_DIR_JSON
    "$ENV{LIBRARIES}/json"
)

set(FETCHCONTENT_SOURCE_DIR_SQLITE
    "$ENV{LIBRARIES}/sqlite-amalgamation-3450100"
)

set(FETCHCONTENT_SOURCE_DIR_TOML
    "$ENV{LIBRARIES}/tomlplusplus"
)

set(GLOW_RESTORE_NUGET
    OFF
)

cmake_path(
    SET
    webview2_SOURCE_DIR
    "$ENV{NUGET_PACKAGES}/microsoft.web.webview2/1.0.2277.86"
)

cmake_path(
    SET
    wil_SOURCE_DIR
    "$ENV{NUGET_PACKAGES}/microsoft.windows.implementationlibrary/1.0.240122.1"
)

cmake_path(
    SET
    cppwinrt_SOURCE_DIR
    "$ENV{NUGET_PACKAGES}/microsoft.windows.cppwinrt/2.0.240111.5"
)
