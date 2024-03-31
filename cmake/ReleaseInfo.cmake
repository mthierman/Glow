file(
    WRITE
    "${PROJECT_BINARY_DIR}/notes/version"
    "v${PROJECT_VERSION}"
)

execute_process(
    COMMAND git rev-parse --short HEAD
    OUTPUT_FILE "${PROJECT_BINARY_DIR}/notes/short_hash"
)

execute_process(
    COMMAND git --no-pager log -5 --oneline --no-decorate
    OUTPUT_FILE "${PROJECT_BINARY_DIR}/notes/release_notes"
)
