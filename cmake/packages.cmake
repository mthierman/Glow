set(SCRIPTS_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(FetchContent)

include(${SCRIPTS_PATH}/versions.cmake)

# Nuget
include(
    ${SCRIPTS_PATH}/deps/wil.cmake
)
include(
    ${SCRIPTS_PATH}/deps/webview.cmake
)
include(
    ${SCRIPTS_PATH}/deps/cppwinrt.cmake
)

# FetchContent
include(
    ${SCRIPTS_PATH}/deps/json.cmake
)
include(
    ${SCRIPTS_PATH}/deps/toml.cmake
)
include(
    ${SCRIPTS_PATH}/deps/sqlite.cmake
)
# include(
#     ${SCRIPTS_PATH}/deps/curl.cmake
# )

# Libraries
include(
    ${SCRIPTS_PATH}/libs/sqlite.cmake
)
include(
    ${SCRIPTS_PATH}/libs/webview.cmake
)
