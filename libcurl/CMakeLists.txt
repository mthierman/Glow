project(
    libcurl
    VERSION 8.4.0
    LANGUAGES C
)

FetchContent_Declare(
    libcurl
    URL ${URL_CURL}
)

FetchContent_MakeAvailable(libcurl)

add_library(${PROJECT_NAME})

add_library(
    libcurl::libcurl
    ALIAS
    ${PROJECT_NAME}
)
