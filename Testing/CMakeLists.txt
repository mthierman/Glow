include(CTest)

enable_testing()

add_executable(Testing)

target_sources(
    Testing
    PRIVATE Tests.cxx
)

target_link_libraries(
    Testing
    PRIVATE glow::glow
            glow::flags
)

add_test(
    NAME Testing
    COMMAND Testing
)

add_executable(Testing_Database)

target_sources(
    Testing_Database
    PRIVATE Database.cxx
)

target_link_libraries(
    Testing_Database
    PRIVATE glow::glow
            glow::flags
)

add_test(
    NAME Testing_Database
    COMMAND Testing_Database
)
