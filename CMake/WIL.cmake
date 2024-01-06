if(GLOW_LOCAL_LIBS)
    file(
        TO_CMAKE_PATH
        "$ENV{NUGET}/${WIL}.${WIL_VERSION}"
        wil_SOURCE_DIR
    )
else()
    execute_process(
        COMMAND
            nuget install ${WIL} -Version ${WIL_VERSION} -OutputDirectory
            ${CMAKE_BINARY_DIR}/_deps/Nuget
    )

    file(
        TO_CMAKE_PATH
        "${CMAKE_BINARY_DIR}/_deps/Nuget/${WIL}.${WIL_VERSION}"
        wil_SOURCE_DIR
    )
endif()
