execute_process(
    COMMAND nuget restore -PackagesDirectory "${CMAKE_BINARY_DIR}/_deps/Nuget"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

cmake_path(
    SET
    webview2_SOURCE_DIR
    "${CMAKE_BINARY_DIR}/_deps/Nuget/Microsoft.Web.WebView2.1.0.2478.35"
)

cmake_path(
    SET
    cppwinrt_SOURCE_DIR
    "${CMAKE_BINARY_DIR}/_deps/Nuget/Microsoft.Windows.CppWinRT.2.0.240405.15"
)

cmake_path(
    SET
    wil_SOURCE_DIR
    "${CMAKE_BINARY_DIR}/_deps/Nuget/Microsoft.Windows.ImplementationLibrary.1.0.240122.1"
)

execute_process(
    COMMAND
        cppwinrt -input "${webview2_SOURCE_DIR}/lib/Microsoft.Web.WebView2.Core.winmd" sdk -output
        "${webview2_SOURCE_DIR}/build/native/include-winrt"
    WORKING_DIRECTORY "${cppwinrt_SOURCE_DIR}/bin"
)
