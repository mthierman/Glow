execute_process(
    COMMAND nuget install ${WEBVIEW} -Version ${WEBVIEW_VER} -OutputDirectory ${NUGET_DIRECTORY}
)

file(
    TO_CMAKE_PATH
    ${NUGET_DIRECTORY}/${WEBVIEW}.${WEBVIEW_VER}
    webview_SOURCE_DIR
)
