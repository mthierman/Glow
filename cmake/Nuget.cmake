cmake_minimum_required(VERSION 3.30)

macro(
    nuget_provide_dependency
    method
    dep_name
    )
    if(${dep_name}
       MATCHES
       "^(Microsoft.Web.WebView2|Microsoft.Windows.CppWinRT|Microsoft.Windows.ImplementationLibrary)$"
        )
        if(EXISTS
           "${CMAKE_BINARY_DIR}/_nuget/${dep_name}"
            )
            return()
        endif()
        message(STATUS "Installing ${dep_name}...")
        execute_process(
            COMMAND
                nuget install ${dep_name} -OutputDirectory "${CMAKE_BINARY_DIR}/_nuget" -Verbosity
                quiet -ExcludeVersion
            )
        fetchcontent_setpopulated(
            ${dep_name}
            SOURCE_DIR
            "${CMAKE_BINARY_DIR}/_nuget/${dep_name}"
            )
    endif()
endmacro()

cmake_language(
    SET_DEPENDENCY_PROVIDER
    nuget_provide_dependency
    SUPPORTED_METHODS
    FETCHCONTENT_MAKEAVAILABLE_SERIAL
    )
