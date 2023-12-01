execute_process(
    COMMAND cppwinrt -input ${webview_SOURCE_DIR}/lib/Microsoft.Web.WebView2.Core.winmd sdk -output ${CMAKE_INCLUDE_OUTPUT_DIRECTORY}
    WORKING_DIRECTORY ${cppwinrt_SOURCE_DIR}/bin
)

file(COPY ${webview_SOURCE_DIR}/build/native/x64/WebView2Loader.dll.lib DESTINATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
file(COPY ${webview_SOURCE_DIR}/build/native/x64/WebView2LoaderStatic.lib DESTINATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

file(COPY ${webview_SOURCE_DIR}/runtimes/win-x64/native/WebView2Loader.dll DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
file(COPY ${webview_SOURCE_DIR}/runtimes/win-x64/native_uap/Microsoft.Web.WebView2.Core.dll DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

file(COPY ${webview_SOURCE_DIR}/build/native/include/WebView2.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})
file(COPY ${webview_SOURCE_DIR}/build/native/include/WebView2EnvironmentOptions.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

file(COPY ${wil_SOURCE_DIR}/include/wil DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})
