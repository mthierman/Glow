add_executable(
    CRTP
    WIN32
)

target_sources(
    CRTP
    PRIVATE app.cxx
            app.hxx
            main.cxx
            window.cxx
            window.hxx
            win32.manifest
)

target_link_libraries(
    CRTP
    PRIVATE glow::glow
            glow::flags
)
