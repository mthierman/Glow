if(GLOW_LOCAL_LIBS)
    file(
        TO_CMAKE_PATH
        "$ENV{LIBRARIES}/curl"
        curl_SOURCE_DIR
    )

    add_subdirectory(
        ${curl_SOURCE_DIR}
        CURL
    )
else()
    FetchContent_Declare(
        curl
        URL https://github.com/curl/curl/releases/download/curl-8_5_0/curl-8.5.0.tar.xz
    )

    FetchContent_MakeAvailable(curl)
endif()
