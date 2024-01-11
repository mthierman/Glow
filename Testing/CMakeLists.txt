enable_testing()

# include(CTest)

add_executable(Glow_Testing)

target_sources(
    Glow_Testing
    PRIVATE Testing.cxx
)

target_link_libraries(
    Glow_Testing
    PRIVATE glow::glow
            glow::flags
)

add_test(
    NAME Glow_Testing
    COMMAND Glow_Testing
)

add_subdirectory(CRTP)
