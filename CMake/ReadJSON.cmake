file(
    READ
    Glow.json
    GLOW_JSON
)

string(
    JSON
    NAME
    GET
    ${GLOW_JSON}
    NAME
)

string(
    JSON
    VERSION
    GET
    ${GLOW_JSON}
    VERSION
)

string(
    JSON
    WEBVIEW
    GET
    ${GLOW_JSON}
    WEBVIEW
)

string(
    JSON
    WEBVIEW_VER
    GET
    ${GLOW_JSON}
    WEBVIEW_VER
)

string(
    JSON
    CPPWINRT
    GET
    ${GLOW_JSON}
    CPPWINRT
)

string(
    JSON
    CPPWINRT_VER
    GET
    ${GLOW_JSON}
    CPPWINRT_VER
)

string(
    JSON
    WIL
    GET
    ${GLOW_JSON}
    WIL
)

string(
    JSON
    WIL_VER
    GET
    ${GLOW_JSON}
    WIL_VER
)

string(
    JSON
    URL_WIL
    GET
    ${GLOW_JSON}
    URL_WIL
)

string(
    JSON
    URL_NLOHMANN_JSON
    GET
    ${GLOW_JSON}
    URL_NLOHMANN_JSON
)

string(
    JSON
    URL_MARZER_TOML
    GET
    ${GLOW_JSON}
    URL_MARZER_TOML
)

string(
    JSON
    URL_SQLITE
    GET
    ${GLOW_JSON}
    URL_SQLITE
)

string(
    JSON
    URL_CURL
    GET
    ${GLOW_JSON}
    URL_CURL
)
