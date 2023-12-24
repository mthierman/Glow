add_executable(IconConverter)

target_sources(
    IconConverter
    PRIVATE IconConverter.cxx
            ${PROJECT_SOURCE_DIR}/Data/glow.manifest
    PRIVATE FILE_SET
            HEADERS
            FILES
            IconConverter.hxx
)

target_link_libraries(
    IconConverter
    PRIVATE glow::glow
            glow::flags
            windowscodecs.lib
)
