# Header
execute_process(
    COMMAND
        cppwinrt -input ${webview_SOURCE_DIR}/lib/Microsoft.Web.WebView2.Core.winmd sdk -output
        ${INCLUDE_DIRECTORY}
    WORKING_DIRECTORY ${cppwinrt_SOURCE_DIR}/bin
)

file(
    COPY ${webview_SOURCE_DIR}/build/native/include/WebView2.h
    DESTINATION ${INCLUDE_DIRECTORY}
)

file(
    COPY ${webview_SOURCE_DIR}/build/native/include/WebView2EnvironmentOptions.h
    DESTINATION ${INCLUDE_DIRECTORY}
)

file(
    COPY ${wil_SOURCE_DIR}/include/wil
    DESTINATION ${INCLUDE_DIRECTORY}
)

# Lib
file(
    COPY ${webview_SOURCE_DIR}/build/native/x64/WebView2Loader.dll.lib
    DESTINATION ${LIB_DIRECTORY}
)

file(
    COPY ${webview_SOURCE_DIR}/build/native/x64/WebView2LoaderStatic.lib
    DESTINATION ${LIB_DIRECTORY}
)

# DLL
file(
    COPY ${webview_SOURCE_DIR}/runtimes/win-x64/native/WebView2Loader.dll
    DESTINATION ${DLL_DIRECTORY}
)

file(
    COPY ${webview_SOURCE_DIR}/runtimes/win-x64/native_uap/Microsoft.Web.WebView2.Core.dll
    DESTINATION ${DLL_DIRECTORY}
)
