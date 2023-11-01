#include <Windows.h>
#include "include/Window.hxx"
#include "include/WebView.hxx"

#define ID_WEBVIEW_1 100
#define ID_WEBVIEW_2 101

class App : public glow::Window
{
  public:
    App(glow::Style, std::optional<HWND>, std::optional<int>);

  private:
    LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);
    int _OnPaint(HWND, UINT, WPARAM, LPARAM);
    static BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lparam);
};

App::App(glow::Style s, std::optional<HWND> h, std::optional<int> i) : glow::Window(s, h, i) {}

LRESULT App::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_PAINT:
        return _OnPaint(hWnd, uMsg, wParam, lParam);
    }

    return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

int App::_OnPaint(HWND hWnd, UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    RECT r;
    ::GetClientRect(hWnd, &r);
    ::EnumChildWindows(hWnd, EnumChildProc, (LPARAM)&r);

    return 0;
}

BOOL CALLBACK App::EnumChildProc(HWND hwnd, LPARAM lparam)
{
    auto childId{::GetWindowLongPtrW(hwnd, GWL_ID)};

    auto rcParent{(LPRECT)lparam};

    if (childId == ID_WEBVIEW_1)
        ::SetWindowPos(hwnd, nullptr, 0, 0, (rcParent->right / 2), rcParent->bottom, SWP_NOZORDER);

    if (childId == ID_WEBVIEW_2)
        ::SetWindowPos(hwnd, nullptr, (rcParent->right / 2), 0, (rcParent->right / 2),
                       rcParent->bottom, SWP_NOZORDER);

    return 1;
}
