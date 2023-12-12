execute_process(
    COMMAND
        nuget install Microsoft.Web.WebView2 -Version 1.0.2151.40 -OutputDirectory
        ${CMAKE_BINARY_DIR}/_deps/Nuget
)
