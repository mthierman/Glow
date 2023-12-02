file(
    TO_CMAKE_PATH
    ${PROJECT_SOURCE_DIR}/data/Glow.manifest
    GLOW_DATA_MANIFEST
)

configure_file(
    ${PROJECT_SOURCE_DIR}/config/GlowConfig.h.in
    ${PROJECT_BINARY_DIR}/config/GlowConfig.h
)

file(
    TO_CMAKE_PATH
    ${PROJECT_BINARY_DIR}/config
    GLOW_CONFIG_PATH
)
