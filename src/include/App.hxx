#include <Windows.h>
#include "Window.hxx"
#include <memory>

class App : public glow::Window
{
  public:
    App(bool);

  private:
    LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);

    int _OnClose(HWND, UINT, WPARAM, LPARAM);
    int _OnDestroy(HWND, UINT, WPARAM, LPARAM);
    int _OnNotify(HWND, UINT, WPARAM, LPARAM);
};

App::App(bool popup) : glow::Window(popup) {}

LRESULT App::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_NOTIFY:
        return _OnNotify(hWnd, uMsg, wParam, lParam);
    }

    return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

int App::_OnClose(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    ::DestroyWindow(hWnd);

    return 0;
}

int App::_OnDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    ::PostQuitMessage(0);

    return 0;
}

int App::_OnNotify(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    ::MessageBoxW(nullptr, L"Virtual WndProc Success!", L"Test", 0);

    return 0;
}
