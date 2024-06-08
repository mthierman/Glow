add_executable(
    Glow_ExampleGui
    WIN32
    )

target_sources(
    Glow_ExampleGui
    PRIVATE "src/gui.cxx"
            "${PROJECT_SOURCE_DIR}/data/glow.manifest"
    )

target_link_libraries(
    Glow_ExampleGui
    PRIVATE glow::glow
            glow::flags
    )

add_custom_command(
    TARGET Glow_ExampleGui
    COMMAND
        ${CMAKE_COMMAND} -E copy "${webview2_SOURCE_DIR}/runtimes/win-x64/native/WebView2Loader.dll"
        "${CMAKE_CURRENT_BINARY_DIR}$<$<CONFIG:DEBUG>:/Debug>$<$<CONFIG:RELEASE>:/Release>"
    COMMENT "Copying WebView2Loader.dll..."
    )

add_custom_command(
    TARGET Glow_ExampleGui
    COMMAND
        ${CMAKE_COMMAND} -E copy
        "${webview2_SOURCE_DIR}/runtimes/win-x64/native_uap/Microsoft.Web.WebView2.Core.dll"
        "${CMAKE_CURRENT_BINARY_DIR}$<$<CONFIG:DEBUG>:/Debug>$<$<CONFIG:RELEASE>:/Release>"
    COMMENT "Copying Microsoft.Web.WebView2.Core.dll..."
    )
