#pragma once

#include <Windows.h>
#include <string>

#include "../helpers/helpers.hxx"

namespace glow
{
struct Window
{
    Window(std::string, Bounds);
    ~Window();
    HWND get_hwnd();
    void* get_hwnd_void();

  private:
    std::string name;
    Bounds bounds;
    HWND hwnd;
    static __int64 __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);
};

Window::Window(std::string n, Bounds b) : name(n), bounds(b)
{
    auto className{glow::win32::randomize_name(name)};

    WNDCLASSEXW wcex{sizeof(WNDCLASSEX)};
    wcex.lpszClassName = className.c_str();
    wcex.lpszMenuName = className.c_str();
    wcex.lpfnWndProc = Window::WndProc;
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = GetModuleHandleW(nullptr);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.hCursor = (HCURSOR)LoadImageW(nullptr, (LPCWSTR)IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
    wcex.hIcon = (HICON)LoadImageW(nullptr, (LPCWSTR)IDI_APPLICATION, IMAGE_ICON, 0, 0,
                                   LR_SHARED | LR_DEFAULTSIZE);
    wcex.hIconSm = (HICON)LoadImageW(nullptr, (LPCWSTR)IDI_APPLICATION, IMAGE_ICON, 0, 0,
                                     LR_SHARED | LR_DEFAULTSIZE);

    auto atom{RegisterClassExW(&wcex)};

    if (atom == 0)
        MessageBoxW(nullptr, std::to_wstring(GetLastError()).c_str(), L"Error", 0);

    hwnd = CreateWindowExW(0, className.c_str(), className.c_str(), WS_OVERLAPPED, CW_USEDEFAULT,
                           CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr,
                           GetModuleHandleW(nullptr), this);

    if (!hwnd)
        MessageBoxW(nullptr, std::to_wstring(GetLastError()).c_str(), L"Error", 0);

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    // SetWindowPos(hwnd, nullptr, 10, 10, 200, 200, 0);
}

Window::~Window() {}

HWND Window::get_hwnd() { return hwnd; }

void* Window::get_hwnd_void() { return (void*)hwnd; }

__int64 __stdcall Window::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    Window* window = InstanceFromWndProc<Window>(hwnd, msg, lparam);

    if (window)
    {
        switch (msg)
        {
        case WM_CLOSE:
            DestroyWindow(hwnd);
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        }
    }

    return DefWindowProcW(hwnd, msg, wparam, lparam);
}
} // namespace glow
