#include <Windows.h>

#include "../helpers/Helpers.hxx"
#include "../gui/App.hxx"
#include "../gui/WebView.hxx"

int WINAPI wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, PWSTR /*pCmdLine*/,
                    int /*nCmdShow*/)
{
    // auto console{glow::create_console()};

    glow::gui::App app("Glow");

    glow::gui::WebView wv("WebView", app.appHwnd, 1);

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

    // glow::remove_console(console);

    return 0;
}
