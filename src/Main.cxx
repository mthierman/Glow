#include <Windows.h>
#include "include/MainWindow.hxx"
#include "include/PopupWindow.hxx"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    glow::MainWindow mainWindow{"Glow"};

    glow::PopupWindow popupWindow{"Settings"};

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
