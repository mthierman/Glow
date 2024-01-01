add_executable(
    Example_App
    WIN32
)

target_sources(
    Example_App
    PRIVATE App/App.cxx
            ${PROJECT_SOURCE_DIR}/Data/glow.manifest
)

target_link_libraries(
    Example_App
    PRIVATE glow::glow
            glow::flags
)

add_executable(Example_Console)

target_sources(
    Example_Console
    PRIVATE Console/Console.cxx
            ${PROJECT_SOURCE_DIR}/Data/glow.manifest
)

target_link_libraries(
    Example_Console
    PRIVATE glow::glow
            glow::flags
)

add_custom_command(
    TARGET Example_App
    COMMAND
        ${CMAKE_COMMAND} -E copy ${webview2_SOURCE_DIR}/runtimes/win-x64/native/WebView2Loader.dll
        ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Copying WebView2Loader.dll..."
)

add_custom_command(
    TARGET Example_App
    COMMAND
        ${CMAKE_COMMAND} -E copy
        ${webview2_SOURCE_DIR}/runtimes/win-x64/native_uap/Microsoft.Web.WebView2.Core.dll
        ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Copying Microsoft.Web.WebView2.Core.dll..."
)

add_executable(
    Example_Test
    WIN32
)

target_sources(
    Example_Test
    PRIVATE Test/Test.cxx
            ${PROJECT_SOURCE_DIR}/Data/glow.manifest
)

target_link_libraries(
    Example_Test
    PRIVATE glow::glow
            glow::flags
)
