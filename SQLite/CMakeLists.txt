add_library(SQLite)

add_library(
    sqlite::sqlite
    ALIAS
    SQLite
)

target_sources(
    SQLite
    PRIVATE ${sqlite_SOURCE_DIR}/sqlite3.c
    PUBLIC FILE_SET
           HEADERS
           BASE_DIRS
           ${sqlite_SOURCE_DIR}
           FILES
           ${sqlite_SOURCE_DIR}/sqlite3.h
)

target_link_libraries(
    SQLite
    PRIVATE glow::features
            glow::flags
)
