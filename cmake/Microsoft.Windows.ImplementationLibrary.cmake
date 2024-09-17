FetchContent_Declare(
    Microsoft.Windows.ImplementationLibrary
    URL "https://www.nuget.org/api/v2/package/Microsoft.Windows.ImplementationLibrary/1.0.240803.1"
    DOWNLOAD_NO_PROGRESS TRUE
    SOURCE_SUBDIR
    "NULL"
    )

FetchContent_MakeAvailable(Microsoft.Windows.ImplementationLibrary)

add_library(
    microsoft_wil
    INTERFACE
    )

add_library(
    microsoft::wil
    ALIAS
    microsoft_wil
    )

target_sources(
    microsoft_wil
    INTERFACE FILE_SET
              HEADERS
              BASE_DIRS
              "${microsoft_wil_SOURCE_DIR}/include"
    )
