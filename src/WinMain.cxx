#include <Windows.h>

#include "../gui/App.hxx"
#include "../gui/WebView.hxx"

#define ID_WEBVIEW_1 100
#define ID_WEBVIEW_2 101

int WINAPI wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, PWSTR /*pCmdLine*/,
                    int /*nCmdShow*/)
{
    glow::gui::App app("Glow");

    glow::gui::WebView wv("WebView", app.appHwnd, ID_WEBVIEW_1);

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
