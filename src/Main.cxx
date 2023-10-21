#include <Windows.h>
#include "include/Window.hxx"
#include <memory>

class App : public glow::Window
{
  public:
    App(bool);

  private:
    LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);
    int _OnNotify(HWND, UINT, WPARAM, LPARAM);
};

App::App(bool popup) : glow::Window(popup) {}

LRESULT App::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_NOTIFY:
        return this->_OnNotify(hWnd, uMsg, wParam, lParam);
    }

    return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

int App::_OnNotify(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    ::MessageBoxW(nullptr, L"Virtual WndProc Success!", L"Test", 0);

    return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // auto window = glow::Window(false);
    // auto app = App(false);
    // auto w{AppWindow(false)};
    // auto w{AppWindow(false)};
    // auto w{std::make_unique<AppWindow>(false)};

    auto app{App(false)};

    SendMessage(app.m_hWnd, WM_NOTIFY, 0, 0);

    MSG msg;
    int r;

    while ((r = GetMessageW(&msg, nullptr, 0, 0)) != 0)
    {
        if (r == -1)
            return 0;

        else
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    return 0;
}
