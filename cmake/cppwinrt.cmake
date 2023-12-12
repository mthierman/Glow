execute_process(
    COMMAND
        nuget install Microsoft.Windows.CppWinRT -Version 2.0.230706.1 -OutputDirectory
        ${CMAKE_BINARY_DIR}/_deps/Nuget
)
