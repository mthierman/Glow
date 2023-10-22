#include <Windows.h>
#include "include/App.hxx"
#include "include/WebView.hxx"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    App app{glow::Style::Main, std::nullopt, std::nullopt};
    app.initialize();

    WebView wv1{glow::Style::Child, app.m_hWnd, ID_CHILD_1};
    wv1.initialize();

    WebView wv2{glow::Style::Child, app.m_hWnd, ID_CHILD_2};
    wv2.initialize();

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
