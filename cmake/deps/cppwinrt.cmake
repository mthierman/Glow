execute_process(
    COMMAND
    nuget install ${CPPWINRT} -Version ${CPPWINRT_VER} -OutputDirectory ${NUGET_DIRECTORY}
)
file(TO_CMAKE_PATH ${NUGET_DIRECTORY}/${CPPWINRT}.${CPPWINRT_VER} cppwinrt_SOURCE_DIR)
