add_executable(
    Glow_ExampleWin32
    WIN32
)

target_sources(
    Glow_ExampleWin32
    PRIVATE "win32.cxx"
            "${PROJECT_SOURCE_DIR}/data/glow.manifest"
)

target_link_libraries(
    Glow_ExampleWin32
    PRIVATE glow::glow
            glow::flags
)

add_executable(Glow_ExampleConsole)

target_sources(
    Glow_ExampleConsole
    PRIVATE "console.cxx"
            "${PROJECT_SOURCE_DIR}/data/glow.manifest"
)

target_link_libraries(
    Glow_ExampleConsole
    PRIVATE glow::glow
            glow::flags
)

add_executable(
    Glow_ExampleWindow
    WIN32
)

target_sources(
    Glow_ExampleWindow
    PRIVATE "window/window.cxx"
            "${PROJECT_SOURCE_DIR}/data/glow.manifest"
)

target_link_libraries(
    Glow_ExampleWindow
    PRIVATE glow::glow
            glow::flags
)

add_executable(
    Glow_ExampleErrors
    WIN32
)

target_sources(
    Glow_ExampleErrors
    PRIVATE "errors/errors.cxx"
            "${PROJECT_SOURCE_DIR}/data/glow.manifest"
)

target_link_libraries(
    Glow_ExampleErrors
    PRIVATE glow::glow
            glow::flags
)

add_custom_command(
    TARGET Glow_ExampleWin32
    COMMAND
        ${CMAKE_COMMAND} -E copy "${webview2_SOURCE_DIR}/runtimes/win-x64/native/WebView2Loader.dll"
        "${CMAKE_CURRENT_BINARY_DIR}$<$<CONFIG:DEBUG>:/Debug>$<$<CONFIG:RELEASE>:/Release>"
    COMMENT "Copying WebView2Loader.dll..."
)

add_custom_command(
    TARGET Glow_ExampleWin32
    COMMAND
        ${CMAKE_COMMAND} -E copy
        "${webview2_SOURCE_DIR}/runtimes/win-x64/native_uap/Microsoft.Web.WebView2.Core.dll"
        "${CMAKE_CURRENT_BINARY_DIR}$<$<CONFIG:DEBUG>:/Debug>$<$<CONFIG:RELEASE>:/Release>"
    COMMENT "Copying Microsoft.Web.WebView2.Core.dll..."
)
