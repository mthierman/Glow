#include <Windows.h>
#include "include/App.hxx"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    App app{glow::Style::Main, std::nullopt};
    App popup{glow::Style::Popup, std::nullopt};
    App child{glow::Style::Child, app.m_hWnd};

    SendMessage(app.m_hWnd, WM_NOTIFY, 0, 0);

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
