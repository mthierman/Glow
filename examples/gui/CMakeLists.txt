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
            common::flags
    )

add_custom_command(
    TARGET Glow_ExampleGui
    COMMAND
        ${CMAKE_COMMAND} -E copy
        "${CMAKE_BINARY_DIR}/_deps/Microsoft.Web.WebView2/runtimes/win-x64/native/WebView2Loader.dll"
        "$<TARGET_FILE_DIR:Glow_ExampleGui>/WebView2Loader.dll"
    COMMENT "Copying WebView2Loader.dll..."
    )

add_custom_command(
    TARGET Glow_ExampleGui
    COMMAND
        ${CMAKE_COMMAND} -E copy
        "${CMAKE_BINARY_DIR}/_deps/Microsoft.Web.WebView2/runtimes/win-x64/native_uap/Microsoft.Web.WebView2.Core.dll"
        "$<TARGET_FILE_DIR:Glow_ExampleGui>/Microsoft.Web.WebView2.Core.dll"
    COMMENT "Copying Microsoft.Web.WebView2.Core.dll..."
    )
