if(GLOW_LOCAL_LIBS)
    file(
        TO_CMAKE_PATH
        "$ENV{LIBRARIES}/sqlite-amalgamation-3450000"
        sqlite_SOURCE_DIR
    )
else()
    FetchContent_Declare(
        sqlite
        URL https://www.sqlite.org/2024/sqlite-amalgamation-3450000.zip
    )

    FetchContent_MakeAvailable(sqlite)
endif()

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
