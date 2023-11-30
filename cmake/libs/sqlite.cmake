add_library(
    SQLite
    ${sqlite_SOURCE_DIR}/sqlite3.c
    ${sqlite_SOURCE_DIR}/sqlite3.h
)
add_library(sqlite::sqlite ALIAS SQLite)
target_link_libraries(
    SQLite
    PRIVATE
    glow::glow_compile_features
    glow::glow_compile_options
)
set_target_properties(
    SQLite
    PROPERTIES
    LINKER_LANGUAGE C
    OUTPUT_NAME SQLite
)

file(COPY ${sqlite_SOURCE_DIR}/sqlite3.h DESTINATION ${PROJECT_BINARY_DIR}/include)
