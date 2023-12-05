add_library(${PROJECT_NAME})

add_library(
    glow::glow
    ALIAS
    ${PROJECT_NAME}
)

target_sources(
    ${PROJECT_NAME}
    PRIVATE console/console.cxx
            filesystem/database.cxx
            filesystem/filesystem.cxx
            gui/app.cxx
            gui/gui.cxx
            gui/webview.cxx
            gui/webview_composition.cxx
            logging/logging.cxx
            text/text.cxx
)

target_sources(
    ${PROJECT_NAME}
    PUBLIC FILE_SET
           HEADERS
           FILES
           console/console.hxx
           filesystem/database.hxx
           filesystem/filesystem.hxx
           gui/app.hxx
           gui/gui.hxx
           gui/webview_composition.hxx
           gui/webview.hxx
           logging/logging.hxx
           text/text.hxx
)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC glow::glow_compile_features
           glow::glow_compile_options
           glow::glow_include_directories
           glow::glow_link_libraries
)
