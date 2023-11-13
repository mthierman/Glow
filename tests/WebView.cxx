#include <Windows.h>

#include "../gui/app.hxx"
#include "../gui/webview.hxx"
#include "../console/console.hxx"

int WINAPI wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, PWSTR /*pCmdLine*/,
                    int /*nCmdShow*/)
{
#ifdef _DEBUG
    auto console{glow::console::create_console()};
#endif

    auto app{std::make_unique<glow::gui::App>("WebView")};

    auto wv{std::make_unique<glow::gui::WebView>("WebView", app->appHwnd, 1)};

    MSG msg;
    int r;

    while ((r = GetMessage(&msg, nullptr, 0, 0)) != 0)
    {
        if (r == -1) return 0;

        else
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

#ifdef _DEBUG
    glow::console::remove_console(console);
#endif

    return 0;
}
