file(GENERATE OUTPUT ${CMAKE_BINARY_DIR}/_deps/Glow/data/Glow.rc CONTENT "1 ICON \"$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.ico\"")
file(TO_CMAKE_PATH ${CMAKE_BINARY_DIR}/_deps/Glow/data/Glow.rc DATA_RC)
file(TO_CMAKE_PATH ${PROJECT_SOURCE_DIR}/data/Glow.manifest DATA_MANIFEST)
