#pragma once

#include <Windows.h>

#include <string>

#include "../gui/gui.hxx"
#include "../text/text.hxx"

namespace glow::gui
{
struct App
{
    App(std::string n);
    ~App();

    HWND appHwnd;

  private:
    static LRESULT CALLBACK WndProcCallback(HWND, UINT, WPARAM, LPARAM);
    virtual LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);

    virtual int OnPaint(HWND);
    virtual int OnClose(HWND);
    virtual int OnDestroy();
    virtual int OnWindowPosChanged(HWND);

    static BOOL EnumChildProc(HWND, LPARAM);
};

App::App(std::string n)
{
    auto brush{reinterpret_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH))};

    auto cursor{
        reinterpret_cast<HCURSOR>(::LoadImage(nullptr, reinterpret_cast<LPCSTR>(IDC_ARROW),
                                              IMAGE_CURSOR, 0, 0, LR_SHARED | LR_DEFAULTSIZE))};
    auto defaultIcon{
        reinterpret_cast<HICON>(::LoadImage(nullptr, reinterpret_cast<LPCSTR>(IDI_APPLICATION),
                                            IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE))};

    auto icon{reinterpret_cast<HICON>(::LoadImage(::GetModuleHandle(nullptr), MAKEINTRESOURCE(1),
                                                  IMAGE_ICON, 0, 0, LR_DEFAULTSIZE))};

    WNDCLASSEX wcex{sizeof(WNDCLASSEX)};
    wcex.lpszClassName = n.c_str();
    wcex.lpszMenuName = n.c_str();
    wcex.lpfnWndProc = App::WndProcCallback;
    wcex.style = 0;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = ::GetModuleHandle(nullptr);
    wcex.hbrBackground = brush;
    wcex.hCursor = cursor;
    wcex.hIcon = icon ? icon : defaultIcon;
    wcex.hIconSm = icon ? icon : defaultIcon;

    ::RegisterClassEx(&wcex);

    ::CreateWindowEx(0, n.c_str(), n.c_str(), WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, CW_USEDEFAULT,
                     CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr,
                     ::GetModuleHandle(nullptr), this);

    ::ShowWindow(appHwnd, SW_SHOWDEFAULT);
};

App::~App() {}

LRESULT CALLBACK App::WndProcCallback(HWND h, UINT m, WPARAM w, LPARAM l)
{
    App* app = InstanceFromWndProc<App, &App::appHwnd>(h, m, l);

    if (app)
    {
        switch (m)
        {
        case WM_PAINT: return app->OnPaint(h);
        case WM_CLOSE: return app->OnClose(h);
        case WM_DESTROY: return app->OnDestroy();
        case WM_WINDOWPOSCHANGED: return app->OnWindowPosChanged(h);
        }

        return app->WndProc(h, m, w, l);
    }

    return ::DefWindowProc(h, m, w, l);
}

LRESULT App::WndProc(HWND h, UINT m, WPARAM w, LPARAM l) { return ::DefWindowProc(h, m, w, l); }

int App::OnPaint(HWND h)
{
    RECT r;
    ::GetClientRect(h, &r);
    ::EnumChildWindows(h, EnumChildProc, (LPARAM)&r);

    return 0;
}

int App::OnClose(HWND h)
{
    ::DestroyWindow(h);

    return 0;
}

int App::OnDestroy()
{
    ::PostQuitMessage(0);

    return 0;
}

int App::OnWindowPosChanged(HWND h)
{
    RECT appRect{0, 0, 0, 0};
    ::GetWindowRect(h, &appRect);
    ::SetWindowPos(::GetConsoleWindow(), nullptr, appRect.left, appRect.bottom,
                   (appRect.right - appRect.left), 200, SWP_NOZORDER | SWP_ASYNCWINDOWPOS);

    return 0;
}

BOOL CALLBACK App::EnumChildProc(HWND h, LPARAM l)
{
    auto childId{::GetWindowLongPtr(h, GWL_ID)};

    auto rcParent{(LPRECT)l};

    if (childId == 1)
        ::SetWindowPos(h, nullptr, 0, 0, (rcParent->right - rcParent->left),
                       (rcParent->bottom - rcParent->top), SWP_NOZORDER);

    return 1;
}
} // namespace glow::gui
