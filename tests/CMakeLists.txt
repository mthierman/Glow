enable_testing()

# include(CTest)

add_executable(Glow_Tests)

target_sources(
    Glow_Tests
    PRIVATE tests.cxx
)

target_link_libraries(
    Glow_Tests
    PRIVATE glow::glow
            glow::flags
)

add_test(
    NAME Glow_Tests
    COMMAND Glow_Tests
)

add_executable(
    CRTP
    WIN32
)

target_sources(
    CRTP
    PRIVATE crtp/main.cxx
            crtp/app.cxx
            crtp/app.hxx
            crtp/browser.cxx
            crtp/browser.hxx
            crtp/definitions.hxx
            crtp/window.cxx
            crtp/window.hxx
            crtp/win32.manifest
            crtp/win32.rc
)

target_link_libraries(
    CRTP
    PRIVATE glow::glow
            glow::flags
)
