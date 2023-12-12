execute_process(
    COMMAND
        nuget install ${CPPWINRT} -Version ${CPPWINRT_VER} -OutputDirectory
        ${CMAKE_BINARY_DIR}/_deps/Nuget
)
