add_executable(Glow_ExampleConsole)

target_sources(
    Glow_ExampleConsole
    PRIVATE "src/console.cxx"
            "${PROJECT_SOURCE_DIR}/data/glow.manifest"
    )

target_link_libraries(
    Glow_ExampleConsole
    PRIVATE glow::glow
            common::flags
    )
