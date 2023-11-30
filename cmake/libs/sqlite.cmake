set(SQLITE_NAME "SQLite")
add_library(
    ${SQLITE_NAME}
    ${sqlite_SOURCE_DIR}/sqlite3.c
    ${sqlite_SOURCE_DIR}/sqlite3.h
)
# add_library(sqlite::sqlite ALIAS ${SQLITE_NAME})
target_link_libraries(
    ${SQLITE_NAME}
    PRIVATE
    glow::glow_compile_features
    glow::glow_compile_options
)
set_target_properties(
    ${SQLITE_NAME}
    PROPERTIES
    LINKER_LANGUAGE C
    OUTPUT_NAME ${SQLITE_NAME}
)
