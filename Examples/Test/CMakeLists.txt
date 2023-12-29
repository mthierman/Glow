add_executable(
    Test
    WIN32
)

target_sources(
    Test
    PRIVATE Test.cxx
            App.hxx
            Browser.hxx
            ${PROJECT_SOURCE_DIR}/Data/glow.manifest
)

target_link_libraries(
    Test
    PRIVATE glow::glow
            glow::flags
            glow::definitions
)
