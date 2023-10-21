#include <Windows.h>
#include "include/App.hxx"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    App app{glow::Style::Main, std::nullopt};

    app.initialize();

    WebView wv{glow::Style::Child, app.m_hWnd};

    wv.initialize();

    // SendMessage(app.m_hWnd, WM_SIZE, 0, 0);

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
