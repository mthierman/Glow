set(NUGET_DIRECTORY
    ${CMAKE_BINARY_DIR}/_deps/Nuget
)

execute_process(
    COMMAND nuget install ${WIL} -Version ${WIL_VER} -OutputDirectory ${NUGET_DIRECTORY}
)
file(
    TO_CMAKE_PATH
    ${NUGET_DIRECTORY}/${WIL}.${WIL_VER}
    wil_SOURCE_DIR
)

execute_process(
    COMMAND nuget install ${WEBVIEW} -Version ${WEBVIEW_VER} -OutputDirectory ${NUGET_DIRECTORY}
)
file(
    TO_CMAKE_PATH
    ${NUGET_DIRECTORY}/${WEBVIEW}.${WEBVIEW_VER}
    webview_SOURCE_DIR
)

execute_process(
    COMMAND nuget install ${CPPWINRT} -Version ${CPPWINRT_VER} -OutputDirectory ${NUGET_DIRECTORY}
)
file(
    TO_CMAKE_PATH
    ${NUGET_DIRECTORY}/${CPPWINRT}.${CPPWINRT_VER}
    cppwinrt_SOURCE_DIR
)

execute_process(
    COMMAND
        cppwinrt -input ${webview_SOURCE_DIR}/lib/Microsoft.Web.WebView2.Core.winmd sdk -output
        ${webview_SOURCE_DIR}/build/native/include-winrt
    WORKING_DIRECTORY ${cppwinrt_SOURCE_DIR}/bin
)

# FetchContent
FetchContent_Declare(
    NLOHMANN_JSON
    URL ${URL_NLOHMANN_JSON}
)
FetchContent_MakeAvailable(NLOHMANN_JSON)

FetchContent_Declare(
    MARZER_TOML
    URL ${URL_MARZER_TOML}
)
FetchContent_MakeAvailable(MARZER_TOML)
