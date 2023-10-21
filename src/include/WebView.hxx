#include <Windows.h>
#include "Window.hxx"

class WebView : public glow::Window
{
  public:
    WebView(glow::Style, std::optional<HWND>);

  private:
    // LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);

    // int _OnWindowPosChanged(HWND, UINT, WPARAM, LPARAM);
};

WebView::WebView(glow::Style s, std::optional<HWND> h) : glow::Window(s, h) {}

// LRESULT WebView::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
// {
//     switch (uMsg)
//     {
//     case WM_WINDOWPOSCHANGED:
//         return _OnWindowPosChanged(hWnd, uMsg, wParam, lParam);
//     }

//     return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
// }

// int WebView::_OnWindowPosChanged(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
// {
//     ::MessageBoxW(nullptr, L"WebView", L"Test", 0);

//     return 0;
// }
