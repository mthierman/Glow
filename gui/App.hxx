#pragma once

#include <Windows.h>

#include <string>

#include "../include/Helpers.hxx"

namespace glow::gui
{
struct App
{
    App(std::string);
    ~App();

    HWND appHwnd;

  private:
    static LRESULT CALLBACK WndProcCallback(HWND, UINT, WPARAM, LPARAM);
    virtual LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);

    virtual int OnPaint(HWND);
    virtual int OnClose(HWND);
    virtual int OnDestroy();

    static BOOL EnumChildProc(HWND, LPARAM);
};

App::App(std::string n)
{
    auto brush{reinterpret_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH))};
    auto cursor{
        reinterpret_cast<HCURSOR>(::LoadImageW(nullptr, reinterpret_cast<LPCWSTR>(IDC_ARROW),
                                               IMAGE_CURSOR, 0, 0, LR_SHARED | LR_DEFAULTSIZE))};
    auto icon{
        reinterpret_cast<HICON>(::LoadImageW(nullptr, reinterpret_cast<LPCWSTR>(IDI_APPLICATION),
                                             IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE))};

    WNDCLASSEXW wcex{sizeof(WNDCLASSEX)};
    wcex.lpszClassName = L"App";
    wcex.lpszMenuName = L"App";
    wcex.lpfnWndProc = App::WndProcCallback;
    wcex.style = 0;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = ::GetModuleHandleW(nullptr);
    wcex.hbrBackground = brush;
    wcex.hCursor = cursor;
    wcex.hIcon = icon;
    wcex.hIconSm = icon;

    ::RegisterClassExW(&wcex);

    ::CreateWindowExW(0, L"App", widen(n).c_str(), WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
                      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr,
                      ::GetModuleHandleW(nullptr), this);

    ShowWindow(appHwnd, SW_SHOWDEFAULT);
};

App::~App() {}

LRESULT CALLBACK App::WndProcCallback(HWND h, UINT m, WPARAM w, LPARAM l)
{
    App* app = InstanceFromWndProc<App, &App::appHwnd>(h, m, l);

    if (app)
    {
        switch (m)
        {
        case WM_PAINT:
            return app->OnPaint(h);
        case WM_CLOSE:
            return app->OnClose(h);
        case WM_DESTROY:
            return app->OnDestroy();
        }

        return app->WndProc(h, m, w, l);
    }

    return ::DefWindowProcW(h, m, w, l);
}

LRESULT App::WndProc(HWND h, UINT m, WPARAM w, LPARAM l) { return ::DefWindowProcW(h, m, w, l); }

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

BOOL CALLBACK App::EnumChildProc(HWND h, LPARAM l)
{
    auto childId{::GetWindowLongPtrW(h, GWL_ID)};

    auto rcParent{(LPRECT)l};

    if (childId == 1)
        ::SetWindowPos(h, nullptr, 0, 0, (rcParent->right - rcParent->left),
                       (rcParent->bottom - rcParent->top), SWP_NOZORDER);

    return 1;
}
} // namespace glow::gui
