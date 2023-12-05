add_library(
    glow_glow_common
    INTERFACE
)

add_library(
    glow::common
    ALIAS
    glow_glow_common
)

target_compile_features(
    glow_glow_common
    INTERFACE c_std_17
              cxx_std_23
)

target_compile_options(
    glow_glow_common
    INTERFACE $<$<CXX_COMPILER_ID:MSVC>:
              /W4
              /WX
              /wd4100
              /bigobj
              /diagnostics:caret
              /Zc:__cplusplus>
)

target_link_options(
    glow_glow_common
    INTERFACE
    $<$<CXX_COMPILER_ID:MSVC>:
    /WX>
)
