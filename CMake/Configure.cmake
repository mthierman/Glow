configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Config/GlowConfig.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/Config/GlowConfig.h
)

file(
    TO_CMAKE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/Data/Glow.manifest
    GLOW_DATA_MANIFEST
)

file(
    TO_CMAKE_PATH
    ${CMAKE_CURRENT_BINARY_DIR}/Config
    GLOW_CONFIG
)

option(
    ENABLE_GLOW_ICONCONVERTER
    OFF
)
option(
    ENABLE_GLOW_EXAMPLES
    OFF
)
option(
    ENABLE_GLOW_TESTING
    OFF
)
option(
    ENABLE_GLOW_GENERATE_EXAMPLE_ICONS
    OFF
)
