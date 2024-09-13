project(example_webview)

add_executable(
    ${PROJECT_NAME}_overlapped
    WIN32
    )

target_sources(
    ${PROJECT_NAME}_overlapped
    PRIVATE "overlapped.cxx"
            "../../data/glow.manifest"
    )

add_executable(
    ${PROJECT_NAME}_split
    WIN32
    )

target_sources(
    ${PROJECT_NAME}_split
    PRIVATE "split.cxx"
            "../../data/glow.manifest"
    )

target_link_libraries(
    ${PROJECT_NAME}_overlapped
    PRIVATE common::compile_features
            common::compile_definitions
            common::link_options_exe
            glow::glow
            glow::compile_options
    )

target_link_libraries(
    ${PROJECT_NAME}_split
    PRIVATE common::compile_features
            common::compile_definitions
            common::link_options_exe
            glow::glow
            glow::compile_options
    )
