#pragma once

#include <Windows.h>
#include <string>
#include <optional>
#include "Helpers.hxx"

namespace glow
{
enum class Style
{
    Main,
    Popup,
    Child
};

class Window
{
  public:
    Window(Style, std::optional<HWND>);
    ~Window();

    void show();
    void hide();
    void focus();

    Style style;
    HWND m_hWnd;
    HWND m_parenthWnd;
    ATOM m_atom;
    HBRUSH m_hBrush;

  private:
    static LRESULT CALLBACK WndProcCallback(HWND, UINT, WPARAM, LPARAM);
    virtual LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);

    int _OnClose(HWND, UINT, WPARAM, LPARAM);
    int _OnDestroy(HWND, UINT, WPARAM, LPARAM);
    // int _OnPaint(HWND, UINT, WPARAM, LPARAM);
};

Window::Window(Style s, std::optional<HWND> h) : style(s)
{
    auto className{glow::randomize(L"Window")};

    auto hInstance{::GetModuleHandleW(nullptr)};

    auto hIcon{reinterpret_cast<HICON>(::LoadImageW(hInstance, glow::widen("APP_ICON").c_str(),
                                                    IMAGE_ICON, 0, 0, LR_DEFAULTSIZE))};

    auto hCursor{reinterpret_cast<HCURSOR>(::LoadImageW(
        nullptr, reinterpret_cast<LPCWSTR>(IDC_ARROW), IMAGE_CURSOR, 0, 0, LR_SHARED))};

    switch (style)
    {
    case Style::Main:
        m_hBrush = reinterpret_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));
        break;

    case Style::Popup:
        m_hBrush = reinterpret_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH));
        break;

    case Style::Child:
        m_hBrush = reinterpret_cast<HBRUSH>(::GetStockObject(LTGRAY_BRUSH));
        break;
    }

    WNDCLASSEXW wcex{sizeof(WNDCLASSEX)};
    wcex.lpszClassName = className.c_str();
    wcex.lpszMenuName = className.c_str();
    wcex.lpfnWndProc = Window::WndProcCallback;
    wcex.style = 0;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    // wcex.hbrBackground = (style == Style::Child) ? m_hBrush : 0;
    wcex.hbrBackground = m_hBrush;
    wcex.hCursor = hCursor;
    wcex.hIcon = hIcon;
    wcex.hIconSm = hIcon;

    m_atom = ::RegisterClassExW(&wcex);

    if (m_atom == 0)
        ::MessageBoxW(nullptr, std::to_wstring(::GetLastError()).c_str(), L"Error", 0);

    switch (style)
    {
    case Style::Main:
        ::CreateWindowExW(0, className.c_str(), glow::widen(APP_NAME).c_str(),
                          WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT,
                          CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr,
                          ::GetModuleHandleW(nullptr), this);
        break;

    case Style::Popup:
        ::CreateWindowExW(0, className.c_str(), glow::widen(APP_NAME).c_str(),
                          WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
                          CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr,
                          nullptr, ::GetModuleHandleW(nullptr), this);
        break;

    case Style::Child:
        m_parenthWnd = h.value();
        ::CreateWindowExW(0, className.c_str(), glow::widen(APP_NAME).c_str(),
                          WS_CHILD | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                          CW_USEDEFAULT, m_parenthWnd, nullptr, ::GetModuleHandleW(nullptr), this);
        break;
    }

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
        default:
            return pWindow->WndProc(hWnd, uMsg, wParam, lParam);
        }
    }

    else
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
    if (style == Style::Main)
        ::PostQuitMessage(0);

    return 0;
}

// int Window::_OnPaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
// {
//     PAINTSTRUCT ps;
//     HDC hdc{::BeginPaint(hWnd, &ps)};
//     ::FillRect(hdc, &ps.rcPaint, m_hBrush);
//     ::EndPaint(hWnd, &ps);

//     return 0;
// }
} // namespace glow
