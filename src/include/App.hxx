#include <Windows.h>
#include "Window.hxx"

class App : public glow::Window
{
  public:
    App(glow::Style, std::optional<HWND>);

  private:
    LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);

    int _OnNotify(HWND, UINT, WPARAM, LPARAM);
    int _OnSize(HWND, UINT, WPARAM, LPARAM);

    static BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lparam);
};

App::App(glow::Style s, std::optional<HWND> h) : glow::Window(s, h) {}

LRESULT App::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_NOTIFY:
        return _OnNotify(hWnd, uMsg, wParam, lParam);
    case WM_SIZE:
        return _OnSize(hWnd, uMsg, wParam, lParam);
    }

    return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

int App::_OnNotify(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    ::MessageBoxW(nullptr, L"OnNotify", L"Test", 0);

    return 0;
}

int App::_OnSize(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // ::MessageBoxW(nullptr, L"OnSize", L"Test", 0);
    RECT r;
    GetClientRect(hWnd, &r);
    EnumChildWindows(hWnd, EnumChildProc, (LPARAM)&r);

    return 0;
}

BOOL CALLBACK App::EnumChildProc(HWND hwnd, LPARAM lparam)
{
    auto child{GetWindowLongPtrW(hwnd, GWL_ID)};
    auto p{(LPRECT)lparam};

    SetWindowPos(hwnd, nullptr, 0, 0, (p->right / 2), p->bottom, SWP_NOZORDER);

    return 1;
}
