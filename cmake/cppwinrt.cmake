execute_process(
    COMMAND
    nuget install ${CPPWINRT} -Version ${CPPWINRT_VER} -OutputDirectory ${NUGET_PATH}
)
file(TO_CMAKE_PATH ${NUGET_PATH}/${CPPWINRT}.${CPPWINRT_VER} cppwinrt_SOURCE_DIR)
