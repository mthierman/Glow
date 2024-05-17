add_library(Ada)

add_library(
    ada::ada
    ALIAS
    Ada
)

target_sources(
    Ada
    PRIVATE "${PROJECT_SOURCE_DIR}/libs/ada-url/ada/ada.cpp"
    PUBLIC FILE_SET
           HEADERS
           BASE_DIRS
           "${PROJECT_SOURCE_DIR}/libs/ada-url/ada"
           FILES
           "${PROJECT_SOURCE_DIR}/libs/ada-url/ada/ada.h"
)

target_link_libraries(
    Ada
    PRIVATE glow::definitions
            glow::features
            glow::flags_deps
)
