execute_process(
    COMMAND
    nuget install ${WEBVIEW} -Version ${WEBVIEW_VER} -OutputDirectory ${CMAKE_NUGET_OUTPUT_DIRECTORY}
)
file(TO_CMAKE_PATH ${CMAKE_NUGET_OUTPUT_DIRECTORY}/${WEBVIEW}.${WEBVIEW_VER} webview_SOURCE_DIR)
