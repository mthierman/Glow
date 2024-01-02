add_library(
    glow_features
    INTERFACE
)

add_library(
    glow::features
    ALIAS
    glow_features
)

target_compile_features(
    glow_features
    INTERFACE c_std_17
              cxx_std_23
)

add_library(
    glow_definitions
    INTERFACE
)

add_library(
    glow::definitions
    ALIAS
    glow_definitions
)

target_compile_definitions(
    glow_definitions
    INTERFACE NOMINMAX
              WIN32_LEAN_AND_MEAN
              NLOHMANN_JSON_NAMESPACE_NO_VERSION=1
              GDIPVER=0x0110
)

add_library(
    glow_flags
    INTERFACE
)

add_library(
    glow::flags
    ALIAS
    glow_flags
)

target_compile_options(
    glow_flags
    INTERFACE $<$<CXX_COMPILER_ID:MSVC>:
              /W4
              /WX
              /wd4100
              /wd4189
              /bigobj
              /diagnostics:caret
              /Zc:__cplusplus
              >
)

target_link_options(
    glow_flags
    INTERFACE
    $<$<CXX_COMPILER_ID:MSVC>:
    /entry:mainCRTStartup
    /WX
    >
)
