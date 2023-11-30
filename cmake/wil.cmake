execute_process(
    COMMAND
    nuget install ${WIL} -Version ${WIL_VER} -OutputDirectory ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Nuget
)
file(TO_CMAKE_PATH ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Nuget/${WIL}.${WIL_VER} wil_SOURCE_DIR)

# FetchContent_Declare(
#     wil
#     URL ${URL_WIL}
# )
# FetchContent_Populate(wil)
