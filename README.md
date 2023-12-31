<img src="./Data/banner.svg" width="200">

# C++ Library for Windows

-   App & WebView2 classes
-   Filesystem functions
-   Text handling
-   Attachable console

## Requirements:

-   C++ 23 Compiler (MSVC/Clang)
-   Nuget package manager CLI
    -   Included with Visual Studio
    -   https://www.nuget.org/downloads
    -   winget CLI (part of App Installer) (https://apps.microsoft.com/detail/9NBLGGH4NNS1)
        -   `winget install Microsoft.NuGet`

## Example usage

### Git submodule:

```pwsh
git submodule add https://github.com/mthierman/Glow.git libs/Glow
```

```cmake
add_subdirectory(libs/Glow)

add_executable(
    ${PROJECT_NAME}
    main.cxx
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    glow::glow
)
```

### CMake FetchContent:

```cmake
include(FetchContent)

FetchContent_Declare(
    glow
    URL https://github.com/mthierman/Glow/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(glow)

add_executable(
    ${PROJECT_NAME}
    main.cxx
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    glow::glow
)
```

To use a recommended set of MSVC flags, optionally link to `glow::flags`
