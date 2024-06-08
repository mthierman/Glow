add_executable(Glow_Tests)

target_sources(Glow_Tests PRIVATE "colors.cxx")

target_link_libraries(
    Glow_Tests
    PRIVATE glow::glow
            glow::flags
    )

add_test(NAME Glow_Tests COMMAND Glow_Tests)
