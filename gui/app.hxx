#pragma once

#include "GlowConfig.h"

#include <Windows.h>

#include <string>
#include <print>

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
    static auto CALLBACK WndProcCallback(HWND h, UINT m, WPARAM w, LPARAM l) -> LRESULT;
    virtual auto WndProc(HWND h, UINT m, WPARAM w, LPARAM l) -> LRESULT;

    auto OnClose(HWND h) -> int;
    auto OnDestroy() -> int;
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

auto CALLBACK App::WndProcCallback(HWND h, UINT m, WPARAM w, LPARAM l) -> LRESULT
{
    App* app = InstanceFromWndProc<App, &App::appHwnd>(h, m, l);

    if (app)
    {
        switch (m)
        {
        case WM_CLOSE: return app->OnClose(h);
        case WM_DESTROY: return app->OnDestroy();
        default: return app->WndProc(h, m, w, l);
        }
    }

    return ::DefWindowProc(h, m, w, l);
}

auto App::OnClose(HWND h) -> int
{
    ::DestroyWindow(h);

    return 0;
}

auto App::OnDestroy() -> int
{
    ::PostQuitMessage(0);

    return 0;
}

auto App::WndProc(HWND h, UINT m, WPARAM w, LPARAM l) -> LRESULT
{
    return ::DefWindowProc(h, m, w, l);
}
} // namespace glow::gui
