if(GLOW_BUILD_EXAMPLES)
    file(
        GENERATE
        OUTPUT ${PROJECT_BINARY_DIR}/Glow.rc
        CONTENT "1 ICON \"$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.ico\""
    )

    file(
        TO_CMAKE_PATH
        ${PROJECT_BINARY_DIR}/Glow.rc
        DATA_RC
    )
endif()

file(
    TO_CMAKE_PATH
    ${PROJECT_SOURCE_DIR}/data/Glow.manifest
    DATA_MANIFEST
)
