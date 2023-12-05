project(
    libcurl
    VERSION 8.4.0
    LANGUAGES C
)

FetchContent_Declare(
    ${PROJECT_NAME}
    URL ${URL_CURL}
)

FetchContent_MakeAvailable(${PROJECT_NAME})
