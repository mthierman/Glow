execute_process(
    COMMAND
        nuget install ${WEBVIEW2} -Version ${WEBVIEW2_VER} -OutputDirectory
        ${CMAKE_BINARY_DIR}/_deps/Nuget
)
