file(GENERATE OUTPUT ${DATA_DIRECTORY}/Glow.rc CONTENT "1 ICON \"$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.ico\"")
file(TO_CMAKE_PATH ${DATA_DIRECTORY}/Glow.rc DATA_RC)
file(TO_CMAKE_PATH ${PROJECT_SOURCE_DIR}/data/Glow.manifest DATA_MANIFEST)
