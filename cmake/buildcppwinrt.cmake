execute_process(
    COMMAND
        cppwinrt -input ${webview2_SOURCE_DIR}/lib/Microsoft.Web.WebView2.Core.winmd sdk -output
        ${webview2_SOURCE_DIR}/build/native/include-winrt
    WORKING_DIRECTORY ${cppwinrt_SOURCE_DIR}/bin
)
