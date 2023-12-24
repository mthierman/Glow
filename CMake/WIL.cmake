if(GLOW_LOCAL_LIBS)
    file(
        TO_CMAKE_PATH
        "$ENV{NUGET}/${WIL}.${WIL_VER}"
        wil_SOURCE_DIR
    )
else()
    execute_process(
        COMMAND
            nuget install ${WIL} -Version ${WIL_VER} -OutputDirectory
            ${CMAKE_BINARY_DIR}/_deps/Nuget
    )

    file(
        TO_CMAKE_PATH
        "${CMAKE_BINARY_DIR}/_deps/Nuget/${WIL}.${WIL_VER}"
        wil_SOURCE_DIR
    )
endif()
