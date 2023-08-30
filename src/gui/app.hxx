#pragma once

#include <Windows.h>
#include <string>

#include "../helpers/helpers.hxx"

namespace glow
{
class App
{
  public:
    App(std::string, bool, Bounds);
    ~App();

    HWND get_hwnd();
    void* get_hwnd_void();

  private:
    static __int64 __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);

    std::string appName;
    std::string randomName;
    Bounds bounds;
    ATOM atom;
    HWND hwnd;
};

App::App(std::string n, bool plugin, Bounds b)
    : appName(n), randomName(glow::win32::randomize(n)), bounds(b)
{
    auto wideName{glow::win32::to_wstring(appName)};
    auto wideRandomName{glow::win32::to_wstring(randomName)};

    WNDCLASSEXW wcex{sizeof(WNDCLASSEX)};
    wcex.lpszClassName = wideRandomName.c_str();
    wcex.lpszMenuName = wideRandomName.c_str();
    wcex.lpfnWndProc = App::WndProc;
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

    atom = RegisterClassExW(&wcex);

    if (atom == 0)
        MessageBoxW(nullptr, std::to_wstring(GetLastError()).c_str(), L"Error", 0);

    hwnd = CreateWindowExW(0, wideRandomName.c_str(), wideName.c_str(),
                           plugin ? WS_POPUP : WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                           CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr,
                           GetModuleHandleW(nullptr), this);

    if (!hwnd)
        MessageBoxW(nullptr, std::to_wstring(GetLastError()).c_str(), L"Error", 0);

    if (!(bounds.width == 0) || !(bounds.height == 0))
        SetWindowPos(hwnd, nullptr, bounds.x, bounds.y, bounds.width, bounds.height, 0);

    ShowWindow(hwnd, SW_SHOWDEFAULT);
}

App::~App() {}

__int64 __stdcall App::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    App* app = InstanceFromWndProc<App>(hwnd, msg, lparam);

    if (app)
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

HWND App::get_hwnd() { return hwnd; }

void* App::get_hwnd_void() { return (void*)hwnd; }
} // namespace glow
