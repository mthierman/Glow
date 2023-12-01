add_library(SQLite)

add_library(
    sqlite::sqlite
    ALIAS
    SQLite
)

target_sources(
    SQLite
    PRIVATE ${sqlite_SOURCE_DIR}/sqlite3.c
            ${sqlite_SOURCE_DIR}/sqlite3.h
)

target_link_libraries(
    SQLite
    PRIVATE glow::glow_compile_features
            glow::glow_compile_options
)

set_target_properties(
    SQLite
    PROPERTIES OUTPUT_NAME
               SQLite
)

file(
    COPY ${sqlite_SOURCE_DIR}/sqlite3.h
    DESTINATION ${INCLUDE_DIRECTORY}
)
