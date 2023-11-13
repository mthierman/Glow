#include <Windows.h>

#include "../helpers/Helpers.hxx"
#include "../gui/App.hxx"
#include "../gui/WebView.hxx"

#define IDM_AUDIO_IN 1001
#define IDM_AUDIO_OUT 1002
#define IDM_MIDI_IN 1003
#define IDM_MIDI_OUT 1004
#define IDM_SAVE_STATE 1005
#define IDM_LOAD_STATE 1006
#define IDM_RESET_STATE 1007

int WINAPI wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, PWSTR /*pCmdLine*/,
                    int /*nCmdShow*/)
{
    auto console{glow::create_console()};

    glow::gui::App app("Glow");

    glow::gui::WebView wv("WebView", app.appHwnd, 1);

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

    glow::remove_console(console);

    return 0;
}
