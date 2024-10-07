project(example_programs)

add_library(
    ${PROJECT_NAME}_programs
    INTERFACE
    )

add_library(
    glow::programs
    ALIAS
    ${PROJECT_NAME}_programs
    )

target_compile_options(
    ${PROJECT_NAME}_programs
    INTERFACE $<$<CXX_COMPILER_ID:MSVC>:
              /MP
              /utf-8
              /bigobj
              /diagnostics:caret
              /Zc:__cplusplus
              >
              $<$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>:
              /W4
              /WX
              >
              $<$<CXX_COMPILER_FRONTEND_VARIANT:GNU>:
              -Wall
              -Werror
              >
    )

target_link_options(
    ${PROJECT_NAME}_programs
    INTERFACE
    $<$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>:
    /WX
    >
    $<$<CXX_COMPILER_FRONTEND_VARIANT:GNU>:
    -Wl,/WX
    >
    )

target_link_libraries(${PROJECT_NAME}_programs INTERFACE glow::glow)

add_executable(${PROJECT_NAME})

target_sources(
    ${PROJECT_NAME}
    PRIVATE "main.cxx"
            "../../data/glow.manifest"
    )

target_link_libraries(${PROJECT_NAME} PRIVATE glow::programs)
