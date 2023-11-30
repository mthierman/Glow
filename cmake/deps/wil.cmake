execute_process(
    COMMAND
    nuget install ${WIL} -Version ${WIL_VER} -OutputDirectory ${NUGET_PATH}
)
file(TO_CMAKE_PATH ${NUGET_PATH}/${WIL}.${WIL_VER} wil_SOURCE_DIR)

# FetchContent_Declare(
#     wil
#     URL ${URL_WIL}
# )
# FetchContent_Populate(wil)
