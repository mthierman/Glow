file(GENERATE OUTPUT ${PROJECT_BINARY_DIR}/data/res.rc CONTENT "1 ICON \"$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.ico\"")
file(TO_CMAKE_PATH ${PROJECT_BINARY_DIR}/data/res.rc RC_FILE)

if(GLOW_BUILD_ICONCONVERTER)
    set(PNG_FILE ${PROJECT_SOURCE_DIR}/data/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.png)
    set(ICO_FILE ${PROJECT_BINARY_DIR}/data/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.ico)
    add_custom_command(
        OUTPUT ${ICO_FILE}
        COMMAND IconConverter ${PNG_FILE} ${ICO_FILE}
        COMMENT "Running IconConverter..."
    )
    add_custom_target(
        MakeIcon ALL
        DEPENDS ${ICO_FILE}
    )
else()
    set(ICO_SOURCE_FILE ${PROJECT_SOURCE_DIR}/data/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.ico)
    set(ICO_FILE ${PROJECT_BINARY_DIR}/data/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.ico)
    add_custom_command(
        OUTPUT ${ICO_FILE}
        COMMAND ${CMAKE_COMMAND} -E copy ${ICO_SOURCE_FILE} ${PROJECT_BINARY_DIR}
        COMMENT "Copying icon..."
    )
    add_custom_target(
        MakeIcon ALL
        DEPENDS ${ICO_FILE}
    )
endif()

add_library(glow_glow_examples INTERFACE)
add_library(glow::glow_examples ALIAS glow_glow_examples)
target_link_libraries(
    glow_glow_examples
    INTERFACE
    glow::glow
    glow::glow_compile_features
    glow::glow_compile_options
    glow::glow_include_directories
    glow::glow_link_libraries
)

add_executable(
    App
    WIN32
    ${PROJECT_SOURCE_DIR}/examples/App.cxx
    ${PROJECT_SOURCE_DIR}/glow/console/console.hxx
    ${PROJECT_SOURCE_DIR}/glow/gui/app.hxx
    ${RC_FILE}
    ${MANIFEST_FILE}
)
target_link_libraries(App PRIVATE glow::glow_examples)

add_executable(
    Console
    ${PROJECT_SOURCE_DIR}/examples/Console.cxx
    ${PROJECT_SOURCE_DIR}/glow/filesystem/filesystem.hxx
    ${PROJECT_SOURCE_DIR}/glow/logging/logging.hxx
    ${PROJECT_SOURCE_DIR}/glow/text/text.hxx
    ${RC_FILE}
    ${MANIFEST_FILE}
)
target_link_libraries(Console PRIVATE glow::glow_examples)

add_executable(
    WebView
    WIN32
    ${PROJECT_SOURCE_DIR}/examples/WebView.cxx
    ${PROJECT_SOURCE_DIR}/examples/WebView.hxx
    ${PROJECT_SOURCE_DIR}/glow/console/console.hxx
    ${PROJECT_SOURCE_DIR}/glow/gui/app.hxx
    ${PROJECT_SOURCE_DIR}/glow/gui/webview.hxx
    ${RC_FILE}
    ${MANIFEST_FILE}
)
target_link_libraries(WebView PRIVATE glow::glow_examples)

# add_executable(
#     WebViewComposition
#     WIN32
#     ${PROJECT_SOURCE_DIR}/examples/WebViewComposition.cxx
#     ${PROJECT_SOURCE_DIR}/examples/WebView.hxx
#     ${PROJECT_SOURCE_DIR}/glow/console/console.hxx
#     ${PROJECT_SOURCE_DIR}/glow/gui/app.hxx
#     ${PROJECT_SOURCE_DIR}/glow/gui/webview_composition.hxx
#     ${RC_FILE}
#     ${MANIFEST_FILE}
# )
# target_link_libraries(WebViewComposition PRIVATE glow::glow_examples)

add_dependencies(App MakeIcon)
add_dependencies(Console MakeIcon)
add_dependencies(WebView MakeIcon)
# add_dependencies(WebViewComposition MakeIcon)
