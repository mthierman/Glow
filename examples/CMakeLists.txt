add_library(
    ${PROJECT_NAME}_examples
    INTERFACE
    )

target_compile_options(
    ${PROJECT_NAME}_examples
    INTERFACE $<$<CXX_COMPILER_ID:MSVC>:
              /MP
              >
              $<$<CXX_COMPILER_ID:Clang>:
              -Wno-braced-scalar-init
              -Wno-nonportable-include-path
              >
              $<$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>:
              /W4
              /WX
              /nologo
              /utf-8
              /bigobj
              /diagnostics:caret
              /Zc:__cplusplus
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
    /NOLOGO
    /entry:mainCRTStartup
    >
    $<$<CXX_COMPILER_FRONTEND_VARIANT:GNU>:
    -Wl,/WX,/NOLOGO,/entry:mainCRTStartup
    >
    )

add_subdirectory("console")
add_subdirectory("webview")
add_subdirectory("window")
