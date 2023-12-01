file(GENERATE OUTPUT ${PROJECT_BINARY_DIR}/data/Glow.rc CONTENT "1 ICON \"$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.ico\"")
file(TO_CMAKE_PATH ${PROJECT_BINARY_DIR}/data/Glow.rc DATA_RC)
file(TO_CMAKE_PATH ${PROJECT_SOURCE_DIR}/data/Glow.manifest DATA_MANIFEST)
