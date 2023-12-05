project(
    SQLite
    VERSION 3.44.2
    LANGUAGES C
)

add_library(${PROJECT_NAME})

add_library(
    sqlite::sqlite
    ALIAS
    ${PROJECT_NAME}
)

target_sources(
    ${PROJECT_NAME}
    PRIVATE ${sqlite_SOURCE_DIR}/sqlite3.c
    PUBLIC FILE_SET
           HEADERS
           BASE_DIRS
           ${sqlite_SOURCE_DIR}
           FILES
           ${sqlite_SOURCE_DIR}/sqlite3.h
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE glow::glow_compile_features
            glow::glow_compile_options
)
