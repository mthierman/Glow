file(
    WRITE
    ${PROJECT_BINARY_DIR}/Glow_version.txt
    v${PROJECT_VERSION}
)

execute_process(
    COMMAND
        git --no-pager log -5 --oneline --no-decorate
    OUTPUT_FILE ${PROJECT_BINARY_DIR}/Glow_release_notes.txt
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

if(GLOW_BUILD_EXAMPLES)
    file(
        GENERATE
        OUTPUT ${DATA_DIRECTORY}/Glow.rc
        CONTENT "1 ICON \"$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.ico\""
    )

    file(
        TO_CMAKE_PATH
        ${DATA_DIRECTORY}/Glow.rc
        DATA_RC
    )
endif()

file(
    TO_CMAKE_PATH
    ${PROJECT_SOURCE_DIR}/data/Glow.manifest
    DATA_MANIFEST
)

configure_file(
    ${PROJECT_SOURCE_DIR}/config/GlowConfig.h.in
    ${INCLUDE_DIRECTORY}/GlowConfig.h
)
