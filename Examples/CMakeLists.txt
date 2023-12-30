add_executable(
    App
    WIN32
)

target_sources(
    App
    PRIVATE App/App.cxx
            ${PROJECT_SOURCE_DIR}/Data/glow.manifest
)

target_link_libraries(
    App
    PRIVATE glow::glow
            glow::flags
)

add_executable(Console)

target_sources(
    Console
    PRIVATE Console/Console.cxx
            ${PROJECT_SOURCE_DIR}/Data/glow.manifest
)

target_link_libraries(
    Console
    PRIVATE glow::glow
            glow::flags
)

add_custom_command(
    TARGET App
    COMMAND
        ${CMAKE_COMMAND} -E copy ${webview2_SOURCE_DIR}/runtimes/win-x64/native/WebView2Loader.dll
        ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Copying WebView2Loader.dll..."
)

add_custom_command(
    TARGET App
    COMMAND
        ${CMAKE_COMMAND} -E copy
        ${webview2_SOURCE_DIR}/runtimes/win-x64/native_uap/Microsoft.Web.WebView2.Core.dll
        ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Copying Microsoft.Web.WebView2.Core.dll..."
)
