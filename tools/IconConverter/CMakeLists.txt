project(
    IconConverter
    VERSION 0.0.1
    LANGUAGES CXX
)

add_executable(IconConverter)

target_sources(
    IconConverter
    PRIVATE IconConverter.cxx
            ${DATA_MANIFEST}
)

target_sources(
    IconConverter
    PRIVATE FILE_SET
            HEADERS
            FILES
            IconConverter.hxx
)

target_link_libraries(
    IconConverter
    PRIVATE glow::glow
            windowscodecs.lib
)
