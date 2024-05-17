add_library(
    JSON
    INTERFACE
)

add_library(
    nlohmann_json::nlohmann_json
    ALIAS
    JSON
)

target_sources(
    JSON
    PUBLIC FILE_SET
           HEADERS
           BASE_DIRS
           "${PROJECT_SOURCE_DIR}/libs/nlohmann/json/single_include/nlohmann"
           FILES
           "${PROJECT_SOURCE_DIR}/libs/nlohmann/json/single_include/nlohmann/json.hpp"
           "${PROJECT_SOURCE_DIR}/libs/nlohmann/json/single_include/nlohmann/json_fwd.hpp"
)
