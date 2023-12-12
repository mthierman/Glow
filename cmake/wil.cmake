execute_process(
    COMMAND
        nuget install Microsoft.Windows.ImplementationLibrary -Version 1.0.231028.1
        -OutputDirectory ${CMAKE_BINARY_DIR}/_deps/Nuget
)
