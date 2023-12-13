execute_process(
    COMMAND
        cppwinrt -input
        ${CMAKE_BINARY_DIR}/_deps/Nuget/${WEBVIEW2}.${WEBVIEW2_VER}/lib/Microsoft.Web.WebView2.Core.winmd
        sdk -output
        ${CMAKE_BINARY_DIR}/_deps/Nuget/${WEBVIEW2}.${WEBVIEW2_VER}/build/native/include-winrt
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/_deps/Nuget/${CPPWINRT}.${CPPWINRT_VER}/bin
)
