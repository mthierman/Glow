execute_process(
    COMMAND
    nuget install ${CPPWINRT} -Version ${CPPWINRT_VER} -OutputDirectory ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Nuget
)
file(TO_CMAKE_PATH ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Nuget/${CPPWINRT}.${CPPWINRT_VER} cppwinrt_SOURCE_DIR)
