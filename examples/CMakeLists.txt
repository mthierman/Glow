add_executable(
    Glow_ExampleWin32
    WIN32
)

target_sources(
    Glow_ExampleWin32
    PRIVATE win32.cxx
            ${PROJECT_SOURCE_DIR}/data/glow.manifest
)

target_link_libraries(
    Glow_ExampleWin32
    PRIVATE glow::glow
            glow::flags
)

add_executable(Glow_ExampleConsole)

target_sources(
    Glow_ExampleConsole
    PRIVATE console.cxx
            ${PROJECT_SOURCE_DIR}/data/glow.manifest
)

target_link_libraries(
    Glow_ExampleConsole
    PRIVATE glow::glow
            glow::flags
)

add_custom_command(
    TARGET Glow_ExampleWin32
    COMMAND
        ${CMAKE_COMMAND} -E copy ${webview2_SOURCE_DIR}/runtimes/win-x64/native/WebView2Loader.dll
        ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Copying WebView2Loader.dll..."
)

add_custom_command(
    TARGET Glow_ExampleWin32
    COMMAND
        ${CMAKE_COMMAND} -E copy
        ${webview2_SOURCE_DIR}/runtimes/win-x64/native_uap/Microsoft.Web.WebView2.Core.dll
        ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Copying Microsoft.Web.WebView2.Core.dll..."
)

add_subdirectory(window)
