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

if(CMAKE_CXX_COMPILER_ID
   MATCHES
   "MSVC"
)
    target_compile_options(
        glow_glow_common
        INTERFACE /bigobj
                  /diagnostics:caret
                  /W4
                  /WX
                  /wd4100
                  /Zc:__cplusplus
    )

    target_link_options(
        glow_glow_common
        INTERFACE
        /WX
    )
endif()
