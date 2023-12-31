if(GLOW_LOCAL_LIBS)
    file(
        TO_CMAKE_PATH
        "$ENV{NUGET}/${CPPWINRT}.${CPPWINRT_VER}"
        cppwinrt_SOURCE_DIR
    )
else()
    execute_process(
        COMMAND
            nuget install ${CPPWINRT} -Version ${CPPWINRT_VER} -OutputDirectory
            ${CMAKE_BINARY_DIR}/_deps/Nuget
    )

    file(
        TO_CMAKE_PATH
        "${CMAKE_BINARY_DIR}/_deps/Nuget/${CPPWINRT}.${CPPWINRT_VER}"
        cppwinrt_SOURCE_DIR
    )
endif()
