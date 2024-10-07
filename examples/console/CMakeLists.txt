project(example_console)

add_executable(${PROJECT_NAME})

target_sources(
    ${PROJECT_NAME}
    PRIVATE "main.cxx"
            "../../data/glow.manifest"
    )

target_link_libraries(${PROJECT_NAME} PRIVATE glow::examples)

add_executable(${PROJECT_NAME}_string)

target_sources(
    ${PROJECT_NAME}_string
    PRIVATE "string.cxx"
            "../../data/glow.manifest"
    )

target_link_libraries(${PROJECT_NAME}_string PRIVATE glow::examples)

add_executable(${PROJECT_NAME}_config)

target_sources(
    ${PROJECT_NAME}_config
    PRIVATE "config.cxx"
            "../../data/glow.manifest"
    )

target_link_libraries(${PROJECT_NAME}_config PRIVATE glow::examples)

add_executable(${PROJECT_NAME}_process)

target_sources(
    ${PROJECT_NAME}_process
    PRIVATE "process.cxx"
            "../../data/glow.manifest"
    )

target_link_libraries(${PROJECT_NAME}_process PRIVATE glow::examples)
