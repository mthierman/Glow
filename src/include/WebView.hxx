#include <Windows.h>
#include "Window.hxx"

class WebView : public glow::Window
{
  public:
    WebView(glow::Style, std::optional<HWND>);

  private:
    LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);

    int _OnWindowPosChanged(HWND, UINT, WPARAM, LPARAM);
};

WebView::WebView(glow::Style s, std::optional<HWND> h) : glow::Window(s, h) {}

LRESULT WebView::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
}
