include(CTest)

enable_testing()

add_executable(Tests)

target_sources(
    Tests
    PRIVATE Tests.cxx
            ${GLOW_DATA_MANIFEST}
)

target_link_libraries(
    Tests
    PRIVATE glow::glow
)

add_test(
    NAME Tests
    COMMAND Tests
)
