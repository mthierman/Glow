project(
    nlohmann.json
    VERSION 3.11.3
    LANGUAGES CXX
)

FetchContent_Declare(
    ${PROJECT_NAME}
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)

FetchContent_MakeAvailable(${PROJECT_NAME})

add_library(
    ${PROJECT_NAME}
    INTERFACE
)

add_library(
    json::json
    ALIAS
    ${PROJECT_NAME}
)

target_sources(
    ${PROJECT_NAME}
    PUBLIC FILE_SET
           HEADERS
           BASE_DIRS
           ${${PROJECT_NAME}_SOURCE_DIR}
)
