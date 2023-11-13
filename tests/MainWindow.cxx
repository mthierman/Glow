#include <Windows.h>

#include "../gui/app.hxx"
#include "../gui/webview.hxx"
#include "../console/console.hxx"

int WINAPI wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, PWSTR /*pCmdLine*/,
                    int /*nCmdShow*/)
{
    auto console{glow::console::create_console()};

    auto app{std::make_unique<glow::gui::App>("App")};

    auto wv{std::make_unique<glow::gui::WebView>("WebView1", app->appHwnd, 1)};

    MSG msg;
    int r;

    while ((r = GetMessageW(&msg, nullptr, 0, 0)) != 0)
    {
        if (r == -1) return 0;

        else
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    glow::console::remove_console(console);

    return 0;
}
