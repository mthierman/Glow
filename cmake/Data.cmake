configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config/GlowConfig.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/config/GlowConfig.h
)

file(
    TO_CMAKE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/data/Glow.manifest
    GLOW_DATA_MANIFEST
)

file(
    TO_CMAKE_PATH
    ${CMAKE_CURRENT_BINARY_DIR}/config
    GLOW_CONFIG
)
