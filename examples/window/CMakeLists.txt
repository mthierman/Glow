add_executable(
    Glow_ExampleWindow
    WIN32
    )

target_sources(
    Glow_ExampleWindow
    PRIVATE "src/window.cxx"
            "${PROJECT_SOURCE_DIR}/data/glow.manifest"
    )

target_link_libraries(
    Glow_ExampleWindow
    PRIVATE glow::glow
            common::flags
    )
