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
    name
)

string(
    JSON
    VERSION
    GET
    ${PROJECT_JSON}
    version
)
