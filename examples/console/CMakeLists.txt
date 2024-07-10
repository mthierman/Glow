project(examples_console)

add_executable(${PROJECT_NAME})

target_sources(
    ${PROJECT_NAME}
    PRIVATE "main.cxx"
            "${CMAKE_SOURCE_DIR}/data/glow.manifest"
    )

target_link_libraries(${PROJECT_NAME} PRIVATE glow::glow)
