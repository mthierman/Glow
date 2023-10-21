#include <Windows.h>
#include "Window.hxx"
#include "Helpers.hxx"
// #include "WebView.hxx"

// #define ID_CHILD_1 100
// #define ID_CHILD_2 101

class App : public glow::Window
{
  public:
    App(glow::Style, std::optional<HWND>);

  private:
    LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);
    // int _OnSize(HWND, UINT, WPARAM, LPARAM);
    // static BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lparam);
};

App::App(glow::Style s, std::optional<HWND> h) : glow::Window(s, h) {}

LRESULT App::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // switch (uMsg)
    // {
    // case WM_SIZE:
    //     return _OnSize(hWnd, uMsg, wParam, lParam);
    // }
    OutputDebugStringW(L"App WndProc\n");

    return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

// int App::_OnSize(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
// {
//     RECT r;
//     ::GetClientRect(hWnd, &r);
//     ::EnumChildWindows(hWnd, EnumChildProc, (LPARAM)&r);

//     return 0;
// }

// BOOL CALLBACK App::EnumChildProc(HWND hwnd, LPARAM lparam)
// {
//     auto child{::GetWindowLongPtrW(hwnd, GWL_ID)};
//     auto p{(LPRECT)lparam};

//     ::SetWindowPos(hwnd, nullptr, 0, 0, (p->right / 2), p->bottom, SWP_NOZORDER);

//     return 1;
// }
