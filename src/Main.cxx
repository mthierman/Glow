#include "helpers/helpers.hxx"
#include "gui/app.hxx"
#include "gui/window.hxx"

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    auto mainWindow{glow::App("Glow", {0})};

    auto popupWindow{glow::Window("Popup", {0})};

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
