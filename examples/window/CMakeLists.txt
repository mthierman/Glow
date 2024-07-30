project(example_window)

add_executable(
    ${PROJECT_NAME}
    WIN32
    )

target_sources(
    ${PROJECT_NAME}
    PRIVATE "main.cxx"
            "../../data/glow.manifest"
    )

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE common::compile_features
            common::compile_definitions
            common::compile_options
            common::link_options_main
            glow::glow
    )
