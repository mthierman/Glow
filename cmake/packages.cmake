include(FetchContent)

include(
    ${PROJECT_SOURCE_DIR}/cmake/versions.cmake
)

# Nuget
include(
    ${PROJECT_SOURCE_DIR}/cmake/wil.cmake
)
include(
    ${PROJECT_SOURCE_DIR}/cmake/webview.cmake
)
include(
    ${PROJECT_SOURCE_DIR}/cmake/cppwinrt.cmake
)
include(
    ${PROJECT_SOURCE_DIR}/cmake/runcppwinrt.cmake
)

# FetchContent
include(
    ${PROJECT_SOURCE_DIR}/cmake/json.cmake
)
include(
    ${PROJECT_SOURCE_DIR}/cmake/toml.cmake
)
include(
    ${PROJECT_SOURCE_DIR}/cmake/sqlite.cmake
)
# include(
#     ${PROJECT_SOURCE_DIR}/cmake/curl.cmake
# )
