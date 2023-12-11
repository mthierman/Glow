project(
    marzer.toml
    VERSION 3.4.0
    LANGUAGES C
              CXX
)

FetchContent_Declare(
    ${PROJECT_NAME}
    URL https://github.com/marzer/tomlplusplus/archive/refs/tags/v3.4.0.tar.gz
)

FetchContent_MakeAvailable(${PROJECT_NAME})

add_library(
    ${PROJECT_NAME}
    INTERFACE
)

add_library(
    toml::toml
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
