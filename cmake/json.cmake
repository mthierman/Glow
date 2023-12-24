if(GLOW_LOCAL_LIBS)
    file(
        TO_CMAKE_PATH
        "$ENV{LIBRARIES}/json"
        json_SOURCE_DIR
    )

    add_subdirectory(
        ${json_SOURCE_DIR}
        JSON
    )
else()
    FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
    )

    FetchContent_MakeAvailable(json)
endif()
