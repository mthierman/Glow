FetchContent_Declare(
    Microsoft.Windows.CppWinRT
    URL "https://www.nuget.org/api/v2/package/Microsoft.Windows.CppWinRT/2.0.240405.15"
    DOWNLOAD_NO_PROGRESS TRUE
    SOURCE_SUBDIR
    "NULL"
    )

FetchContent_MakeAvailable(Microsoft.Windows.CppWinRT)

execute_process(
    COMMAND
        cppwinrt -input sdk -output
        "${CMAKE_BINARY_DIR}/include/microsoft/Microsoft.Windows.CppWinRT"
    WORKING_DIRECTORY "${microsoft_cppwinrt_SOURCE_DIR}/bin"
    )

add_library(
    microsoft_cppwinrt
    INTERFACE
    )

add_library(
    microsoft::cppwinrt
    ALIAS
    microsoft_cppwinrt
    )

target_sources(
    microsoft_cppwinrt
    INTERFACE FILE_SET
              HEADERS
              BASE_DIRS
              "${CMAKE_BINARY_DIR}/include/microsoft/Microsoft.Windows.CppWinRT"
    )
