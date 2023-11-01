#include <Windows.h>
#include "App.hxx"
// #include "include/WebView.hxx"

int WINAPI wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, PWSTR /*pCmdLine*/,
                    int /*nCmdShow*/)
{
    SetEnvironmentVariableW(L"WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS",
                            L"--allow-file-access-from-files");

    App app{glow::Style::Main, std::nullopt, std::nullopt};

    WebView wv1{glow::Style::Child, app.m_hWnd, ID_WEBVIEW_1};
    wv1.create();

    WebView wv2{glow::Style::Child, app.m_hWnd, ID_WEBVIEW_2};
    wv2.create();

    MSG msg;
    int r;

    while ((r = GetMessageW(&msg, nullptr, 0, 0)) != 0)
    {
        if (r == -1)
            return 0;

        else
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    return 0;
}
