#include <Windows.h>
#include "Window.hxx"

class WebView : public glow::Window
{
  public:
    WebView(glow::Style, std::optional<HWND>, std::optional<int>);

  private:
    LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);

    int _OnWindowPosChanged(HWND, UINT, WPARAM, LPARAM);
};

WebView::WebView(glow::Style s, std::optional<HWND> h, std::optional<int> i) : glow::Window(s, h, i)
{
}

LRESULT WebView::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
}
