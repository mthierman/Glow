add_executable(
    CRTP
    WIN32
)

target_sources(
    CRTP
    PRIVATE main.cxx
            app.cxx
            app.hxx
            browser.cxx
            browser.hxx
            definitions.hxx
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
