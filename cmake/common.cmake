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
              /bigobj
              /diagnostics:caret
              /Zc:__cplusplus>
)

target_link_options(
    glow_flags
    INTERFACE
    $<$<CXX_COMPILER_ID:MSVC>:
    /WX>
)
