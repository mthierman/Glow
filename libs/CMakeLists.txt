add_subdirectory(common)

# add_subdirectory(libcurl)
add_subdirectory(sqlite)
add_subdirectory(json)
add_subdirectory(toml)
add_subdirectory(wil)
add_subdirectory(cppwinrt)
add_subdirectory(webview2)

execute_process(
    COMMAND
        cppwinrt -input
        ${CMAKE_BINARY_DIR}/_deps/Nuget/${WEBVIEW}.${WEBVIEW_VER}/lib/Microsoft.Web.WebView2.Core.winmd
        sdk -output
        ${CMAKE_BINARY_DIR}/_deps/Nuget/${WEBVIEW}.${WEBVIEW_VER}/build/native/include-winrt
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/_deps/Nuget/${CPPWINRT}.${CPPWINRT_VER}/bin
)
