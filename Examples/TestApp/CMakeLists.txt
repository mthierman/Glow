add_executable(
    TestApp
    WIN32
)

target_sources(
    TestApp
    PRIVATE TestApp.cxx
            App.hxx
            Browser.hxx
            ${PROJECT_SOURCE_DIR}/Data/glow.manifest
)

target_link_libraries(
    TestApp
    PRIVATE glow::glow
            glow::flags
            glow::definitions
)
