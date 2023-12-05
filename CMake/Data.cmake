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
