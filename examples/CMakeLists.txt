if(GLOW_BUILD_ICONCONVERTER)
    set(SOURCE_FILE
        ${PROJECT_SOURCE_DIR}/data/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.png
    )
    set(OUTPUT_FILE
        ${PROJECT_BINARY_DIR}/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.ico
    )

    add_custom_command(
        OUTPUT ${OUTPUT_FILE}
        COMMAND
            IconConverter ${SOURCE_FILE} ${OUTPUT_FILE}
        COMMENT "Running IconConverter..."
    )

    add_custom_target(
        MakeIcon
        ALL
        DEPENDS ${OUTPUT_FILE}
    )
else()
    set(SOURCE_FILE
        ${PROJECT_SOURCE_DIR}/data/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.ico
    )
    set(OUTPUT_FILE
        ${PROJECT_BINARY_DIR}/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.ico
    )

    add_custom_command(
        OUTPUT ${OUTPUT_FILE}
        COMMAND
            ${CMAKE_COMMAND} -E copy ${SOURCE_FILE} ${PROJECT_BINARY_DIR}
        COMMENT "Copying icon..."
    )

    add_custom_target(
        MakeIcon
        ALL
        DEPENDS ${OUTPUT_FILE}
    )
endif()

# glow::glow_examples
add_library(
    glow_glow_examples
    INTERFACE
)

add_library(
    glow::glow_examples
    ALIAS
    glow_glow_examples
)

target_link_libraries(
    glow_glow_examples
    INTERFACE glow::glow
)

# Win32 app
add_executable(
    App
    WIN32
)

target_sources(
    App
    PRIVATE App.cxx
            ${PROJECT_SOURCE_DIR}/glow/console/console.hxx
            ${PROJECT_SOURCE_DIR}/glow/gui/app.hxx
            ${GLOW_DATA_MANIFEST}
            ${GLOW_DATA_RC}
)

target_link_libraries(
    App
    PRIVATE glow::glow_examples
)

# Console app
add_executable(Console)

target_sources(
    Console
    PRIVATE Console.cxx
            ${PROJECT_SOURCE_DIR}/glow/filesystem/filesystem.hxx
            ${PROJECT_SOURCE_DIR}/glow/logging/logging.hxx
            ${PROJECT_SOURCE_DIR}/glow/text/text.hxx
            ${GLOW_DATA_MANIFEST}
            ${GLOW_DATA_RC}
)

target_link_libraries(
    Console
    PRIVATE glow::glow_examples
)

# WebView example
add_executable(
    WebView
    WIN32
)

target_sources(
    WebView
    PRIVATE WebView.cxx
            WebView.hxx
            ${PROJECT_SOURCE_DIR}/glow/console/console.hxx
            ${PROJECT_SOURCE_DIR}/glow/gui/app.hxx
            ${PROJECT_SOURCE_DIR}/glow/gui/webview.hxx
            ${GLOW_DATA_MANIFEST}
            ${GLOW_DATA_RC}
)

target_link_libraries(
    WebView
    PRIVATE glow::glow_examples
)

# WebView composition example
add_executable(
    WebViewComposition
    WIN32
)
target_sources(
    WebViewComposition
    PRIVATE WebViewComposition.cxx
            WebView.hxx
            ${PROJECT_SOURCE_DIR}/glow/console/console.hxx
            ${PROJECT_SOURCE_DIR}/glow/gui/app.hxx
            ${PROJECT_SOURCE_DIR}/glow/gui/webview_composition.hxx
            ${GLOW_DATA_MANIFEST}
            ${GLOW_DATA_RC}
)
target_link_libraries(
    WebViewComposition
    PRIVATE glow::glow_examples
            CoreMessaging.lib
)

add_dependencies(
    App
    MakeIcon
)

add_dependencies(
    Console
    MakeIcon
)

add_dependencies(
    WebView
    MakeIcon
)

add_dependencies(
    WebViewComposition
    MakeIcon
)

add_custom_command(
    TARGET WebView
    COMMAND
        ${CMAKE_COMMAND} -E copy
        ${NUGET_DIRECTORY}/${WEBVIEW}.${WEBVIEW_VER}/runtimes/win-x64/native/WebView2Loader.dll
        ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Copying WebView2Loader.dll..."
)

add_custom_command(
    TARGET WebViewComposition
    COMMAND
        ${CMAKE_COMMAND} -E copy
        ${NUGET_DIRECTORY}/${WEBVIEW}.${WEBVIEW_VER}/runtimes/win-x64/native_uap/Microsoft.Web.WebView2.Core.dll
        ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Copying Microsoft.Web.WebView2.Core.dll..."
)
