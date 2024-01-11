add_executable(
    CRTP
    WIN32
)

target_sources(
    CRTP
    PRIVATE app.cxx
            app.hxx
            main.cxx
            # webview.cxx
            # webview.hxx
            window.cxx
            window.hxx
            win32.manifest
            win32.rc
)

target_link_libraries(
    CRTP
    PRIVATE glow::glow
            glow::flags
)
