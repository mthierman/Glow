set(FETCHCONTENT_SOURCE_DIR_ADA
    "$ENV{LIBRARIES}/singleheader"
)

set(FETCHCONTENT_SOURCE_DIR_JSON
    "$ENV{LIBRARIES}/json"
)

set(FETCHCONTENT_SOURCE_DIR_SQLITE
    "$ENV{LIBRARIES}/sqlite-amalgamation-3450200"
)

cmake_path(
    SET
    webview2_SOURCE_DIR
    "$ENV{NUGET}/Microsoft.Web.WebView2.1.0.2365.46"
)

cmake_path(
    SET
    cppwinrt_SOURCE_DIR
    "$ENV{NUGET}/Microsoft.Windows.CppWinRT.2.0.240111.5"
)

cmake_path(
    SET
    wil_SOURCE_DIR
    "$ENV{NUGET}/Microsoft.Windows.ImplementationLibrary.1.0.240122.1"
)
