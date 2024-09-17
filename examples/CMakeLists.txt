add_library(
    ${PROJECT_NAME}_examples
    INTERFACE
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
