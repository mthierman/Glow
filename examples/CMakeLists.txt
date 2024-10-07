add_library(
    ${PROJECT_NAME}_examples
    INTERFACE
    )

add_library(
    glow::examples
    ALIAS
    ${PROJECT_NAME}_examples
    )

target_compile_options(
    ${PROJECT_NAME}_examples
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
    ${PROJECT_NAME}_examples
    INTERFACE
    $<$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>:
    /WX
    >
    $<$<CXX_COMPILER_FRONTEND_VARIANT:GNU>:
    -Wl,/WX
    >
    )

target_link_libraries(${PROJECT_NAME}_examples INTERFACE glow::glow)

add_subdirectory("console")
add_subdirectory("webview")
add_subdirectory("window")
add_subdirectory("programs")
