#pragma once

#include <Windows.h>
#include <dwmapi.h>
#include <string>
#include "Helpers.hxx"

namespace glow
{
class PopupWindow
{
  public:
    PopupWindow(std::string);
    ~PopupWindow();

    HWND get_hwnd();

  private:
    static LRESULT CALLBACK WndProcCallback(HWND, UINT, WPARAM, LPARAM);
    virtual LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);

    int _OnActivate(HWND);
    int _OnClose(HWND);
    int _OnCreate(HWND);
    int _OnDestroy();
    int _OnSize(HWND);
    int _OnSysCommand(HWND, UINT, WPARAM, LPARAM);

    HWND m_hWnd;
};

PopupWindow::PopupWindow(std::string t)
{
    auto name{glow::widen(t)};
    auto className{glow::widen("PopupWindow")};
    auto hInstance = ::GetModuleHandleW(nullptr);

    auto hIcon{reinterpret_cast<HICON>(::LoadImageW(hInstance, glow::widen("APP_ICON").c_str(),
                                                    IMAGE_ICON, 0, 0, LR_DEFAULTSIZE))};

    auto hCursor{reinterpret_cast<HCURSOR>(::LoadImageW(
        nullptr, reinterpret_cast<LPCWSTR>(IDC_ARROW), IMAGE_CURSOR, 0, 0, LR_SHARED))};

    auto hBrush{reinterpret_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH))};

    WNDCLASSEXW wcex{sizeof(WNDCLASSEX)};
    wcex.lpszClassName = className.c_str();
    wcex.lpszMenuName = className.c_str();
    wcex.lpfnWndProc = PopupWindow::WndProcCallback;
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hbrBackground = hBrush;
    wcex.hCursor = hCursor;
    wcex.hIcon = hIcon;
    wcex.hIconSm = hIcon;

    if (::RegisterClassExW(&wcex) == 0)
        ::MessageBoxW(nullptr, std::to_wstring(::GetLastError()).c_str(), L"Error", 0);

    ::CreateWindowExW(0, className.c_str(), name.c_str(),
                      WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
                      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr,
                      ::GetModuleHandleW(nullptr), this);

    if (!m_hWnd)
        ::MessageBoxW(nullptr, std::to_wstring(::GetLastError()).c_str(), L"Error", 0);

    ::ShowWindow(m_hWnd, SW_SHOWNORMAL);
    SetWindowPos(m_hWnd, 0, 0, 0, 400, 400, 0);
}

PopupWindow::~PopupWindow() {}

HWND PopupWindow::get_hwnd() { return m_hWnd; }

LRESULT CALLBACK PopupWindow::WndProcCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PopupWindow* pPopupWindow =
        InstanceFromWndProc<PopupWindow, PopupWindow, &PopupWindow::m_hWnd>(hWnd, uMsg, lParam);

    if (pPopupWindow)
        return pPopupWindow->WndProc(hWnd, uMsg, wParam, lParam);

    return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

LRESULT PopupWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PopupWindow* pPopupWindow =
        InstanceFromWndProc<PopupWindow, PopupWindow, &PopupWindow::m_hWnd>(hWnd, uMsg, lParam);

    if (pPopupWindow)
    {
        switch (uMsg)
        {
        case WM_ACTIVATE:
            return pPopupWindow->_OnActivate(hWnd);
        case WM_CLOSE:
            return pPopupWindow->_OnClose(hWnd);
        case WM_CREATE:
            return pPopupWindow->_OnCreate(hWnd);
        case WM_DESTROY:
            return pPopupWindow->_OnDestroy();
        case WM_SIZE:
            return pPopupWindow->_OnSize(hWnd);
        }
    }

    return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

int PopupWindow::_OnActivate(HWND hWnd)
{
    // MARGINS m{0, 0, 0, 0};

    // if (!SUCCEEDED(DwmExtendFrameIntoClientArea(hWnd, &m)))
    //     ::MessageBoxW(nullptr, std::to_wstring(::GetLastError()).c_str(), L"Error", 0);

    return 0;
}

int PopupWindow::_OnClose(HWND hWnd)
{
    ::DestroyWindow(hWnd);
    return 0;
}

int PopupWindow::_OnCreate(HWND hWnd)
{
    RECT r;
    GetWindowRect(hWnd, &r);
    // SetWindowPos(hWnd, 0, r.left, r.top, (r.right - r.left), (r.bottom - r.top),
    // SWP_FRAMECHANGED);
    return 0;
}

int PopupWindow::_OnDestroy()
{
    // ::PostQuitMessage(0);
    return 0;
}

int PopupWindow::_OnSize(HWND hWnd)
{
    RECT r;
    ::GetClientRect(hWnd, &r);

    return 0;
}
} // namespace glow
