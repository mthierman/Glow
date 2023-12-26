if(GLOW_LOCAL_LIBS)
    file(
        TO_CMAKE_PATH
        "$ENV{LIBRARIES}/sqlite-amalgamation-3440200"
        sqlite_SOURCE_DIR
    )
else()
    FetchContent_Declare(
        sqlite
        URL https://www.sqlite.org/2023/sqlite-amalgamation-3440200.zip
    )

    FetchContent_MakeAvailable(sqlite)
endif()

add_subdirectory(SQLite)
