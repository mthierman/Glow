FetchContent_Declare(
    sqlite
    URL ${URL_SQLITE}
)
FetchContent_MakeAvailable(sqlite)

add_library(sqlite ${sqlite_SOURCE_DIR}/sqlite3.c ${sqlite_SOURCE_DIR}/sqlite3.h)
set_target_properties(sqlite PROPERTIES LINKER_LANGUAGE C)
