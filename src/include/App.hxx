#include <Windows.h>
#include "Window.hxx"

class App : public glow::Window
{
  public:
    App(glow::Style, std::optional<HWND>);

  private:
    LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);

    int _OnNotify(HWND, UINT, WPARAM, LPARAM);
};

App::App(glow::Style s, std::optional<HWND> h) : glow::Window(s, h) {}

LRESULT App::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_NOTIFY:
        return _OnNotify(hWnd, uMsg, wParam, lParam);
    }

    return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

int App::_OnNotify(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    ::MessageBoxW(nullptr, L"Virtual WndProc Success!", L"Test", 0);

    return 0;
}
