project(
    sqlite
    VERSION 3.44.2
    LANGUAGES C
)

FetchContent_Declare(
    ${PROJECT_NAME}
    URL ${URL_SQLITE}
)

FetchContent_MakeAvailable(${PROJECT_NAME})

add_library(${PROJECT_NAME})

add_library(
    sqlite::sqlite
    ALIAS
    ${PROJECT_NAME}
)

target_sources(
    ${PROJECT_NAME}
    PRIVATE ${${PROJECT_NAME}_SOURCE_DIR}/sqlite3.c
    PUBLIC FILE_SET
           HEADERS
           BASE_DIRS
           ${${PROJECT_NAME}_SOURCE_DIR}
           FILES
           ${${PROJECT_NAME}_SOURCE_DIR}/sqlite3.h
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE glow::features
            glow::flags
)
