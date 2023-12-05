set(NUGET_DIRECTORY
    ${CMAKE_BINARY_DIR}/_deps/Nuget
)

file(
    READ
    ${CMAKE_CURRENT_SOURCE_DIR}/Glow.json
    PROJECT_JSON
)

string(
    JSON
    NAME
    GET
    ${PROJECT_JSON}
    NAME
)

string(
    JSON
    VERSION
    GET
    ${PROJECT_JSON}
    VERSION
)

string(
    JSON
    WEBVIEW
    GET
    ${PROJECT_JSON}
    WEBVIEW
)

string(
    JSON
    WEBVIEW_VER
    GET
    ${PROJECT_JSON}
    WEBVIEW_VER
)

string(
    JSON
    CPPWINRT
    GET
    ${PROJECT_JSON}
    CPPWINRT
)

string(
    JSON
    CPPWINRT_VER
    GET
    ${PROJECT_JSON}
    CPPWINRT_VER
)

string(
    JSON
    WIL
    GET
    ${PROJECT_JSON}
    WIL
)

string(
    JSON
    WIL_VER
    GET
    ${PROJECT_JSON}
    WIL_VER
)

string(
    JSON
    URL_WIL
    GET
    ${PROJECT_JSON}
    URL_WIL
)

string(
    JSON
    URL_JSON
    GET
    ${PROJECT_JSON}
    URL_JSON
)

string(
    JSON
    URL_TOML
    GET
    ${PROJECT_JSON}
    URL_TOML
)

string(
    JSON
    URL_SQLITE
    GET
    ${PROJECT_JSON}
    URL_SQLITE
)

string(
    JSON
    URL_CURL
    GET
    ${PROJECT_JSON}
    URL_CURL
)
