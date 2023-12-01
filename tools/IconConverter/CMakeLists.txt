add_executable(IconConverter)

target_sources(
    IconConverter
    PRIVATE
    IconConverter.cxx
    ${PROJECT_SOURCE_DIR}/data/app.manifest
)

target_sources(
    IconConverter
    PRIVATE
    FILE_SET HEADERS
    FILES
    IconConverter.hxx
)

target_link_libraries(
    IconConverter
    PRIVATE
    glow::glow
    glow::glow_compile_features
    glow::glow_compile_options
    glow::glow_include_directories
    windowscodecs.lib
)
