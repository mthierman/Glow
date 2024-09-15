project(example_window)

add_executable(
    ${PROJECT_NAME}_overlapped
    WIN32
    )

target_sources(
    ${PROJECT_NAME}_overlapped
    PRIVATE "overlapped.cxx"
            "../../data/glow.manifest"
    )

target_link_libraries(${PROJECT_NAME}_overlapped PRIVATE glow::glow)

add_executable(
    ${PROJECT_NAME}_derived
    WIN32
    )

target_sources(
    ${PROJECT_NAME}_derived
    PRIVATE "derived.cxx"
            "../../data/glow.manifest"
    )

target_link_libraries(${PROJECT_NAME}_derived PRIVATE glow::glow)

add_executable(
    ${PROJECT_NAME}_message
    WIN32
    )

target_sources(
    ${PROJECT_NAME}_message
    PRIVATE "message.cxx"
            "../../data/glow.manifest"
    )

target_link_libraries(${PROJECT_NAME}_message PRIVATE glow::glow)
