add_executable(IconConverter)

target_sources(
    IconConverter
    PRIVATE IconConverter.cxx
            IconConverter.hxx
            ${PROJECT_SOURCE_DIR}/glow/console/console.hxx
            ${PROJECT_SOURCE_DIR}/glow/filesystem/filesystem.hxx
            ${PROJECT_SOURCE_DIR}/glow/text/text.hxx
            ${DATA_MANIFEST}
)

target_link_libraries(
    IconConverter
    PRIVATE glow::glow
            glow::glow_compile_features
            glow::glow_compile_options
            glow::glow_include_directories
            windowscodecs.lib
)
