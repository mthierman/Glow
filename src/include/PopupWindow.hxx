#pragma once

#include <Windows.h>
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
    void show();
    void hide();

  private:
    static LRESULT CALLBACK WndProcCallback(HWND, UINT, WPARAM, LPARAM);
    virtual LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);

    int _OnClose(HWND);
    int _OnCreate(HWND);

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

    ::ShowWindow(m_hWnd, SW_SHOW);
    SetWindowPos(m_hWnd, 0, 0, 0, 400, 400, 0);
}

PopupWindow::~PopupWindow() {}

HWND PopupWindow::get_hwnd() { return m_hWnd; }

void PopupWindow::show()
{
    ShowWindow(m_hWnd, SW_SHOW);
    BringWindowToTop(m_hWnd);
}

void PopupWindow::hide() { ShowWindow(m_hWnd, SW_HIDE); }

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
        case WM_CLOSE:
            return pPopupWindow->_OnClose(hWnd);
        }
    }

    return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

int PopupWindow::_OnClose(HWND hWnd)
{
    ShowWindow(m_hWnd, SW_HIDE);

    return 0;
}
} // namespace glow
