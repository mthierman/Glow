include(CTest)

add_executable(
    Tests
    ${PROJECT_SOURCE_DIR}/tests/Tests.cxx
    ${GLOW_HEADERS}
)
target_link_libraries(
    Tests
    PRIVATE
    glow::glow_compile_features
    glow::glow_compile_options
    glow::glow_include_directories
    glow::glow_link_libraries
)
add_test(NAME Tests COMMAND Tests)
