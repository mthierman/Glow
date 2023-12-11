project(
    Microsoft.Web.WebView2
    VERSION 1.0.2151.40
    LANGUAGES CXX
)

execute_process(
    COMMAND
        nuget install ${PROJECT_NAME} -Version ${PROJECT_VERSION} -OutputDirectory
        ${CMAKE_BINARY_DIR}/_deps/Nuget
)

add_library(
    ${PROJECT_NAME}
    INTERFACE
)

add_library(
    webview2::webview2
    ALIAS
    ${PROJECT_NAME}
)

target_sources(
    ${PROJECT_NAME}
    PUBLIC FILE_SET
           HEADERS
           BASE_DIRS
           ${CMAKE_BINARY_DIR}/_deps/Nuget/${PROJECT_NAME}.${PROJECT_VERSION}
)
