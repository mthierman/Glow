add_library(
    ${LIB_NAME}
    ${PROJECT_SOURCE_DIR}/glow/Glow.cxx
    ${GLOW_HEADERS}
)
add_library(glow::glow ALIAS ${LIB_NAME})
target_link_libraries(
    ${LIB_NAME}
    PRIVATE
    glow::glow_compile_features
    glow::glow_compile_options
    glow::glow_include_directories
    glow::glow_link_libraries
)
set_target_properties(
    ${LIB_NAME}
    PROPERTIES
    OUTPUT_NAME ${LIB_NAME}
)
