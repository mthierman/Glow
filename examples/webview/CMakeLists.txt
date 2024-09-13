project(example_webview)

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
            common::link_options_exe
            glow::glow
            glow::compile_options
    )
