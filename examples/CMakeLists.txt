# file( TO_CMAKE_PATH ${PROJECT_BINARY_DIR}/data/Glow.rc GLOW_DATA_RC )

# file( GENERATE OUTPUT ${GLOW_DATA_RC} CONTENT "1 ICON \"Glow.ico\"" )

# if(ENABLE_GLOW_GENERATE_EXAMPLE_ICONS) set(GLOW_ICON_SOURCE
# ${PROJECT_SOURCE_DIR}/data/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.png )

# set(GLOW_ICON ${PROJECT_BINARY_DIR}/data/Glow.ico )

# add_custom_command( OUTPUT ${GLOW_ICON} COMMAND IconConverter ${GLOW_ICON_SOURCE} ${GLOW_ICON}
# COMMENT "Running IconConverter..." )

# add_custom_target( MakeIcon ALL DEPENDS ${GLOW_ICON} ) else() set(GLOW_ICON_SOURCE
# ${PROJECT_SOURCE_DIR}/data/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.ico )

# set(GLOW_ICON ${PROJECT_BINARY_DIR}/data/Glow.ico )

# add_custom_command( OUTPUT ${GLOW_ICON} COMMAND ${CMAKE_COMMAND} -E copy ${GLOW_ICON_SOURCE}
# ${GLOW_ICON} COMMENT "Copying icon..." )

# add_custom_target( MakeIcon ALL DEPENDS ${GLOW_ICON} ) endif()

add_executable(
    App
    WIN32
)
target_sources(
    App
    PRIVATE App.cxx
            # ${PROJECT_SOURCE_DIR}/glow/console/console.hxx ${PROJECT_SOURCE_DIR}/glow/gui/app.hxx
            # ${GLOW_DATA_MANIFEST} ${GLOW_DATA_RC}
)
target_link_libraries(
    App
    PRIVATE glow::glow
            glow::flags
)

add_executable(Console)
target_sources(
    Console
    PRIVATE Console.cxx
            # ${PROJECT_SOURCE_DIR}/glow/filesystem/filesystem.hxx
            # ${PROJECT_SOURCE_DIR}/glow/logging/logging.hxx
            # ${PROJECT_SOURCE_DIR}/glow/text/text.hxx ${GLOW_DATA_MANIFEST} ${GLOW_DATA_RC}
)
target_link_libraries(
    Console
    PRIVATE glow::glow
            glow::flags
)

add_executable(
    WebView
    WIN32
)
target_sources(
    WebView
    PRIVATE WebView.cxx
            WebView.hxx
            # ${PROJECT_SOURCE_DIR}/glow/console/console.hxx ${PROJECT_SOURCE_DIR}/glow/gui/app.hxx
            # ${PROJECT_SOURCE_DIR}/glow/gui/webview.hxx ${GLOW_DATA_MANIFEST} ${GLOW_DATA_RC}
)
target_link_libraries(
    WebView
    PRIVATE glow::glow
            glow::flags
)

add_executable(
    WebViewComposition
    WIN32
)
target_sources(
    WebViewComposition
    PRIVATE WebViewComposition.cxx
            WebView.hxx
            # ${PROJECT_SOURCE_DIR}/glow/console/console.hxx ${PROJECT_SOURCE_DIR}/glow/gui/app.hxx
            # ${PROJECT_SOURCE_DIR}/glow/gui/webview_composition.hxx ${GLOW_DATA_MANIFEST}
            # ${GLOW_DATA_RC}
)
target_link_libraries(
    WebViewComposition
    PRIVATE glow::glow
            glow::flags
            CoreMessaging.lib
)

# add_dependencies( App MakeIcon ) add_dependencies( Console MakeIcon ) add_dependencies( WebView
# MakeIcon ) add_dependencies( WebViewComposition MakeIcon )

add_custom_command(
    TARGET WebView
    COMMAND
        ${CMAKE_COMMAND} -E copy
        ${CMAKE_BINARY_DIR}/_deps/Nuget/${WEBVIEW}.${WEBVIEW_VER}/runtimes/win-x64/native/WebView2Loader.dll
        ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Copying WebView2Loader.dll..."
)

add_custom_command(
    TARGET WebViewComposition
    COMMAND
        ${CMAKE_COMMAND} -E copy
        ${CMAKE_BINARY_DIR}/_deps/Nuget/${WEBVIEW}.${WEBVIEW_VER}/runtimes/win-x64/native_uap/Microsoft.Web.WebView2.Core.dll
        ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Copying Microsoft.Web.WebView2.Core.dll..."
)
