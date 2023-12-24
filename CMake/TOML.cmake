if(GLOW_LOCAL_LIBS)
    file(
        TO_CMAKE_PATH
        "$ENV{LIBRARIES}/tomlplusplus"
        toml_SOURCE_DIR
    )

    add_subdirectory(
        ${toml_SOURCE_DIR}
        TOML
    )
else()
    FetchContent_Declare(
        toml
        URL https://github.com/marzer/tomlplusplus/archive/refs/tags/v3.4.0.tar.gz
    )

    FetchContent_MakeAvailable(toml)
endif()
