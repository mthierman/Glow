add_library(
    sqlite_sqlite
    ${sqlite_SOURCE_DIR}/sqlite3.c
    ${sqlite_SOURCE_DIR}/sqlite3.h
)
add_library(sqlite::sqlite ALIAS sqlite_sqlite)
target_link_libraries(
    sqlite_sqlite
    PRIVATE
    glow::glow_compile_features
    glow::glow_compile_options
)
set_target_properties(sqlite_sqlite PROPERTIES LINKER_LANGUAGE C OUTPUT_NAME "SQLite")
