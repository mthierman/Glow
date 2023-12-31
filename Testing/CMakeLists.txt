enable_testing()

# include(CTest)

add_executable(Testing_Compile)

target_sources(
    Testing_Compile
    PRIVATE Compile.cxx
)

target_link_libraries(
    Testing_Compile
    PRIVATE glow::glow
            glow::flags
)

add_test(
    NAME Testing_Compile
    COMMAND Testing_Compile
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
