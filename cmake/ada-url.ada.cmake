FetchContent_Declare(
    ada-url.ada
    URL "https://github.com/ada-url/ada/releases/download/v2.9.2/singleheader.zip"
    DOWNLOAD_NO_PROGRESS TRUE
    SOURCE_SUBDIR
    "NULL"
    )

FetchContent_MakeAvailable(ada-url.ada)

add_library(ada-url_ada)

add_library(
    ada-url::ada
    ALIAS
    ada-url_ada
    )

target_sources(
    ada-url_ada
    PRIVATE "${ada-url.ada_SOURCE_DIR}/ada.cpp"
    PUBLIC FILE_SET
           HEADERS
           BASE_DIRS
           "${ada-url.ada_SOURCE_DIR}"
    )

target_compile_features(
    ada-url_ada
    PRIVATE c_std_17
            cxx_std_23
    )

target_compile_options(
    ada-url_ada
    PRIVATE $<$<CXX_COMPILER_ID:MSVC>:
            /MP
            /utf-8
            /bigobj
            /diagnostics:caret
            /Zc:__cplusplus
            >
    )
