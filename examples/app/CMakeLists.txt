project(example_app)

add_executable(
    ${PROJECT_NAME}
    WIN32
    )

target_sources(
    ${PROJECT_NAME}
    PRIVATE "main.cxx"
            "../../data/glow.manifest"
    )

target_link_libraries(${PROJECT_NAME} PRIVATE glow::glow)
