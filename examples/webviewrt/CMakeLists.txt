project(example_webviewrt)

add_executable(
    ${PROJECT_NAME}
    WIN32
    )

target_sources(
    ${PROJECT_NAME}
    PRIVATE "main.cxx"
            "../../data/glow.manifest"
    )

target_link_libraries(${PROJECT_NAME} PRIVATE glow::glow)

add_custom_target(
    Microsoft.Web.WebView2.Core.dll
    COMMAND
        ${CMAKE_COMMAND} -E copy
        "${CMAKE_BINARY_DIR}/_deps/Microsoft.Web.WebView2/runtimes/win-x64/native_uap/Microsoft.Web.WebView2.Core.dll"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/Microsoft.Web.WebView2.Core.dll"
    )

add_dependencies(
    ${PROJECT_NAME}
    Microsoft.Web.WebView2.Core.dll
    )
