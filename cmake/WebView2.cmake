if(GLOW_LOCAL_LIBS)
    file(
        TO_CMAKE_PATH
        "$ENV{NUGET}/${WEBVIEW2}.${WEBVIEW2_VERSION}"
        webview2_SOURCE_DIR
    )
else()
    execute_process(
        COMMAND
            nuget install ${WEBVIEW2} -Version ${WEBVIEW2_VERSION} -OutputDirectory
            ${CMAKE_BINARY_DIR}/_deps/Nuget
    )

    file(
        TO_CMAKE_PATH
        "${CMAKE_BINARY_DIR}/_deps/Nuget/${WEBVIEW2}.${WEBVIEW2_VERSION}"
        webview2_SOURCE_DIR
    )
endif()
