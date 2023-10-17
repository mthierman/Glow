#pragma once

#include <Windows.h>
#include <string>
#include <filesystem>
#include "winrt/Windows.Foundation.h"
#include "../helpers/helpers.hxx"
#include "gui/webviewrt.hxx"

namespace glow
{
class Window
{
  public:
    Window(std::string);
    ~Window();

    HWND m_hwnd;
    HWND get_hwnd();

  private:
    static __int64 __stdcall WndProcCallback(HWND, UINT, WPARAM, LPARAM);
    virtual __int64 WndProc(HWND, UINT, WPARAM, LPARAM);
    static int __stdcall EnumChildProcCallback(HWND, LPARAM);
    // virtual int EnumChildProc(HWND, LPARAM);
};

Window::Window(std::string name)
{
    auto wideName{glow::win32::to_wstring(name)};
    auto wideRandomName{glow::win32::to_wstring("MainWindow")};

    auto appIcon{(HICON)LoadImageW(GetModuleHandleW(nullptr),
                                   glow::win32::to_wstring("APP_ICON").c_str(), IMAGE_ICON, 0, 0,
                                   LR_DEFAULTSIZE)};

    auto appCursor{(HCURSOR)LoadImageW(nullptr, (LPCWSTR)IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED)};

    WNDCLASSEXW wcex{sizeof(WNDCLASSEX)};
    wcex.lpszClassName = wideRandomName.c_str();
    wcex.lpszMenuName = wideRandomName.c_str();
    wcex.lpfnWndProc = WndProcCallback;
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = GetModuleHandleW(nullptr);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.hCursor = appCursor;
    wcex.hIcon = appIcon;
    wcex.hIconSm = appIcon;

    auto atom{RegisterClassExW(&wcex)};

    if (atom == 0)
        MessageBoxW(nullptr, std::to_wstring(GetLastError()).c_str(), L"Error", 0);

    CreateWindowExW(0, wideRandomName.c_str(), wideName.c_str(),
                    WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPCHILDREN, CW_USEDEFAULT,
                    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr,
                    GetModuleHandleW(nullptr), this);

    if (!m_hwnd)
        MessageBoxW(nullptr, std::to_wstring(GetLastError()).c_str(), L"Error", 0);

    glow::win32::set_darkmode(m_hwnd);
    glow::win32::set_darktitle();
    glow::win32::set_mica(m_hwnd);
    glow::win32::window_cloak(m_hwnd);

    ShowWindow(m_hwnd, SW_SHOWDEFAULT);

    glow::win32::window_uncloak(m_hwnd);

    auto webview{glow::WebView("WebView", m_hwnd)};
}

Window::~Window() {}

HWND Window::get_hwnd() { return m_hwnd; }

__int64 __stdcall Window::WndProcCallback(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    Window* window = InstanceFromWndProc<Window, Window, &Window::m_hwnd>(hwnd, msg, lparam);

    if (window)
    {
        return window->WndProc(hwnd, msg, wparam, lparam);
    }

    return ::DefWindowProcW(hwnd, msg, wparam, lparam);
}

__int64 __stdcall Window::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    Window* window = InstanceFromWndProc<Window, Window, &Window::m_hwnd>(hwnd, msg, lparam);

    if (window)
    {
        switch (msg)
        {
        case WM_CLOSE:
        {
            DestroyWindow(hwnd);
            return 0;
        }

        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }

        case WM_SIZE:
        {
            RECT r;
            GetClientRect(hwnd, &r);
            EnumChildWindows(hwnd, EnumChildProcCallback, (LPARAM)&r);

            return 0;
        }
        }
    }

    return DefWindowProcW(hwnd, msg, wparam, lparam);
}

int __stdcall Window::EnumChildProcCallback(HWND hwnd, LPARAM lparam)
{
    auto child{GetWindowLongPtrW(hwnd, GWL_ID)};
    auto p{(LPRECT)lparam};

    SetWindowPos(hwnd, nullptr, 0, 0, p->right, p->bottom, SWP_NOZORDER);

    return 1;
}

// int __stdcall Window::EnumChildProcCallback(HWND hwnd, LPARAM lparam)
// {
//     Window* window = InstanceFromWndProc<Window, Window, &Window::m_hwnd>(hwnd, 0, lparam);

//     if (window)
//     {
//         return window->EnumChildProc(hwnd, lparam);
//     }

//     return 1;
// }

// int __stdcall Window::EnumChildProc(HWND hwnd, LPARAM lparam)
// {
//     auto child{GetWindowLongPtrW(hwnd, GWL_ID)};
//     auto p{(LPRECT)lparam};

//     SetWindowPos(hwnd, nullptr, 0, 0, p->right, p->bottom, SWP_NOZORDER);

//     return 1;
// }
} // namespace glow