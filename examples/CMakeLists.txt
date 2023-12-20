add_executable(
    App
    WIN32
)

target_sources(
    App
    PRIVATE App.cxx
            App.hxx
            Window.hxx
            ${PROJECT_SOURCE_DIR}/data/glow.manifest
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
            ${PROJECT_SOURCE_DIR}/data/glow.manifest
)

target_link_libraries(
    Console
    PRIVATE glow::glow
            glow::flags
)

# add_executable( WebViewComposition WIN32 ) target_sources( WebViewComposition PRIVATE
# WebViewComposition.cxx WebView.hxx ) target_link_libraries( WebViewComposition PRIVATE glow::glow
# glow::flags CoreMessaging.lib )

# add_custom_command( TARGET WebView COMMAND ${CMAKE_COMMAND} -E copy
# ${CMAKE_BINARY_DIR}/_deps/Nuget/${WEBVIEW}.${WEBVIEW_VER}/runtimes/win-x64/native/WebView2Loader.dll
# ${CMAKE_CURRENT_BINARY_DIR} COMMENT "Copying WebView2Loader.dll..." )

# add_custom_command( TARGET WebViewComposition COMMAND ${CMAKE_COMMAND} -E copy
# ${CMAKE_BINARY_DIR}/_deps/Nuget/${WEBVIEW}.${WEBVIEW_VER}/runtimes/win-x64/native_uap/Microsoft.Web.WebView2.Core.dll
# ${CMAKE_CURRENT_BINARY_DIR} COMMENT "Copying Microsoft.Web.WebView2.Core.dll..." )
