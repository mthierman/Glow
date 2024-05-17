add_library(SQLite)

add_library(
    sqlite::sqlite
    ALIAS
    SQLite
)

target_sources(
    SQLite
    PRIVATE "${PROJECT_SOURCE_DIR}/libs/sqlite/sqlite3.c"
    PUBLIC FILE_SET
           HEADERS
           BASE_DIRS
           "${PROJECT_SOURCE_DIR}/libs/sqlite/"
           FILES
           "${PROJECT_SOURCE_DIR}/libs/sqlite/sqlite3.h"
)

target_link_libraries(
    SQLite
    PRIVATE glow::definitions
            glow::features
            glow::flags_deps
)
