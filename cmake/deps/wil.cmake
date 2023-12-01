execute_process(
    COMMAND
    nuget install ${WIL} -Version ${WIL_VER} -OutputDirectory ${CMAKE_NUGET_OUTPUT_DIRECTORY}
)
file(TO_CMAKE_PATH ${CMAKE_NUGET_OUTPUT_DIRECTORY}/${WIL}.${WIL_VER} wil_SOURCE_DIR)

# FetchContent_Declare(
#     wil
#     URL ${URL_WIL}
# )
# FetchContent_Populate(wil)
