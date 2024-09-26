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

target_link_libraries(${PROJECT_NAME}_overlapped PRIVATE glow::examples)

add_executable(
    ${PROJECT_NAME}_derived
    WIN32
    )

target_sources(
    ${PROJECT_NAME}_derived
    PRIVATE "derived.cxx"
            "../../data/glow.manifest"
    )

target_link_libraries(${PROJECT_NAME}_derived PRIVATE glow::examples)

add_executable(
    ${PROJECT_NAME}_split
    WIN32
    )

target_sources(
    ${PROJECT_NAME}_split
    PRIVATE "split.cxx"
            "../../data/glow.manifest"
    )

target_link_libraries(${PROJECT_NAME}_split PRIVATE glow::examples)

add_executable(
    ${PROJECT_NAME}_event
    WIN32
    )

target_sources(
    ${PROJECT_NAME}_event
    PRIVATE "event.cxx"
            "../../data/glow.manifest"
    )

target_link_libraries(${PROJECT_NAME}_event PRIVATE glow::examples)
