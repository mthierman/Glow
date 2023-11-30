execute_process(
    COMMAND
    nuget install ${WEBVIEW} -Version ${WEBVIEW_VER} -OutputDirectory ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Nuget
)
file(TO_CMAKE_PATH ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Nuget/${WEBVIEW}.${WEBVIEW_VER} webview_SOURCE_DIR)
