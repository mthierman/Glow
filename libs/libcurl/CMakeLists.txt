project(
    libcurl
    VERSION 8.4.0
    LANGUAGES C
)

FetchContent_Declare(
    ${PROJECT_NAME}
    URL https://github.com/curl/curl/releases/download/curl-8_4_0/curl-8.4.0.tar.xz
)

FetchContent_MakeAvailable(${PROJECT_NAME})
