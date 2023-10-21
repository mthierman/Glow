#pragma once

#include <Windows.h>
#include <string>
#include "Helpers.hxx"

namespace glow
{
class Window
{
  public:
    Window(bool);
    ~Window();

    void show();
    void hide();
    void focus();

    HWND m_hWnd;
    ATOM m_atom;
    HBRUSH m_hBrush;
    bool isPopup;

  private:
    static LRESULT CALLBACK WndProcCallback(HWND, UINT, WPARAM, LPARAM);
    virtual LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);

    int _OnClose(HWND, UINT, WPARAM, LPARAM);
    int _OnDestroy(HWND, UINT, WPARAM, LPARAM);
    int _OnPaint(HWND, UINT, WPARAM, LPARAM);
};

Window::Window(bool popup) : isPopup(popup)
{
    auto className{glow::randomize(L"Window")};

    auto hInstance{::GetModuleHandleW(nullptr)};

    auto hIcon{reinterpret_cast<HICON>(::LoadImageW(hInstance, glow::widen("APP_ICON").c_str(),
                                                    IMAGE_ICON, 0, 0, LR_DEFAULTSIZE))};

    auto hCursor{reinterpret_cast<HCURSOR>(::LoadImageW(
        nullptr, reinterpret_cast<LPCWSTR>(IDC_ARROW), IMAGE_CURSOR, 0, 0, LR_SHARED))};

    m_hBrush = reinterpret_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH));

    WNDCLASSEXW wcex{sizeof(WNDCLASSEX)};
    wcex.lpszClassName = className.c_str();
    wcex.lpszMenuName = className.c_str();
    wcex.lpfnWndProc = Window::WndProcCallback;
    wcex.style = 0;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hbrBackground = 0;
    wcex.hCursor = hCursor;
    wcex.hIcon = hIcon;
    wcex.hIconSm = hIcon;

    m_atom = ::RegisterClassExW(&wcex);

    if (m_atom == 0)
        ::MessageBoxW(nullptr, std::to_wstring(::GetLastError()).c_str(), L"Error", 0);

    ::CreateWindowExW(0, className.c_str(), glow::widen(APP_NAME).c_str(),
                      isPopup ? WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX
                              : WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
                      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr,
                      ::GetModuleHandleW(nullptr), this);

    if (!m_hWnd)
        ::MessageBoxW(nullptr, std::to_wstring(::GetLastError()).c_str(), L"Error", 0);

    ::ShowWindow(m_hWnd, SW_SHOWDEFAULT);
}

Window::~Window() {}

void Window::show() { ShowWindow(m_hWnd, SW_SHOW); }

void Window::hide() { ShowWindow(m_hWnd, SW_HIDE); }

void Window::focus() { BringWindowToTop(m_hWnd); }

LRESULT CALLBACK Window::WndProcCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Window* pWindow = InstanceFromWndProc<Window, &Window::m_hWnd>(hWnd, uMsg, lParam);

    if (pWindow)
    {
        switch (uMsg)
        {
        case WM_CLOSE:
            return pWindow->_OnClose(hWnd, uMsg, wParam, lParam);
        case WM_DESTROY:
            return pWindow->_OnDestroy(hWnd, uMsg, wParam, lParam);
        case WM_PAINT:
            return pWindow->_OnPaint(hWnd, uMsg, wParam, lParam);
        }
        return pWindow->WndProc(hWnd, uMsg, wParam, lParam);
    }

    return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

LRESULT Window::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

int Window::_OnClose(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    ::DestroyWindow(hWnd);

    return 0;
}

int Window::_OnDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    isPopup ? void() : ::PostQuitMessage(0);

    return 0;
}

int Window::_OnPaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc{::BeginPaint(hWnd, &ps)};
    ::FillRect(hdc, &ps.rcPaint, m_hBrush);
    ::EndPaint(hWnd, &ps);

    return 0;
}
} // namespace glow
