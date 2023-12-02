include(CTest)

enable_testing()

add_executable(Tests)

target_sources(
    Tests
    PRIVATE Tests.cxx
            ${DATA_MANIFEST}
)

target_link_libraries(
    Tests
    PRIVATE glow::glow
            glow::glow_compile_features
            glow::glow_compile_options
            glow::glow_include_directories
            glow::glow_link_libraries
)

add_test(
    NAME Tests
    COMMAND
        Tests
)
