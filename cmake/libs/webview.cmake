execute_process(
    COMMAND cppwinrt -input ${webview_SOURCE_DIR}/lib/Microsoft.Web.WebView2.Core.winmd sdk -output ${PROJECT_BINARY_DIR}/include
    WORKING_DIRECTORY ${cppwinrt_SOURCE_DIR}/bin
)
file(COPY ${webview_SOURCE_DIR}/runtimes/win-x64/native_uap/Microsoft.Web.WebView2.Core.dll DESTINATION ${PROJECT_BINARY_DIR})
