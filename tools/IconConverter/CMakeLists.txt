project(
    IconConverter
    VERSION 0.0.1
    LANGUAGES CXX
)

add_executable(${PROJECT_NAME})

target_sources(
    ${PROJECT_NAME}
    PRIVATE IconConverter.cxx
            ${GLOW_DATA_MANIFEST}
)

target_sources(
    ${PROJECT_NAME}
    PRIVATE FILE_SET
            HEADERS
            FILES
            IconConverter.hxx
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE glow::glow
            glow::flags
            windowscodecs.lib
)
