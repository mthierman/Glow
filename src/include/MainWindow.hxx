#pragma once

#include <Windows.h>
#include <string>
// #include <filesystem>
// #include "winrt/Windows.Foundation.h"
// #include "helpers.hxx"

namespace glow
{
template <class T, class U, HWND(U::*m_hWnd)>
T* InstanceFromWndProc(HWND hWnd, UINT uMsg, LPARAM lParam)
{
    T* pInstance;
    if (uMsg == WM_NCCREATE)
    {
        LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
        pInstance = reinterpret_cast<T*>(pCreateStruct->lpCreateParams);
        ::SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pInstance));
        pInstance->*m_hWnd = hWnd;
    }
    else
    {
        pInstance = reinterpret_cast<T*>(::GetWindowLongPtrW(hWnd, GWLP_USERDATA));
    }

    return pInstance;
}

std::string narrow(std::wstring in)
{
    if (!in.empty())
    {
        auto inSize{static_cast<int>(in.size())};

        auto outSize{::WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS | WC_ERR_INVALID_CHARS,
                                           in.data(), inSize, nullptr, 0, nullptr, nullptr)};

        if (outSize > 0)
        {
            std::string out;
            out.resize(static_cast<size_t>(outSize));

            if (::WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS | WC_ERR_INVALID_CHARS,
                                      in.data(), inSize, out.data(), outSize, nullptr, nullptr) > 0)
                return out;
        }
    }

    return {};
}

std::wstring widen(std::string in)
{
    if (!in.empty())
    {
        auto inSize{static_cast<int>(in.size())};

        auto outSize{
            ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, in.data(), inSize, nullptr, 0)};

        if (outSize > 0)
        {
            std::wstring out;
            out.resize(static_cast<size_t>(outSize));

            if (::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, in.data(), inSize, out.data(),
                                      outSize) > 0)
                return out;
        }
    }

    return {};
}

class Window
{
  public:
    Window(std::string);
    ~Window();

    HWND get_hwnd();

  private:
    // static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
    // static BOOL CALLBACK EnumChildProc(HWND, LPARAM);

    static LRESULT CALLBACK WndProcCallback(HWND, UINT, WPARAM, LPARAM);
    virtual LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);
    // static BOOL CALLBACK EnumChildProcCallback(HWND, LPARAM);
    // virtual BOOL EnumChildProc(HWND, LPARAM);

    HWND m_hWnd;
};

Window::Window(std::string t)
{
    auto name{glow::widen(t)};
    auto className{glow::widen("MainWindow")};

    auto hIcon{reinterpret_cast<HICON>(::LoadImageW(::GetModuleHandleW(nullptr),
                                                    glow::widen("APP_ICON").c_str(), IMAGE_ICON, 0,
                                                    0, LR_DEFAULTSIZE))};

    auto hCursor{reinterpret_cast<HCURSOR>(
        ::LoadImageW(nullptr, (LPCWSTR)IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED))};

    auto hBrush{reinterpret_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH))};

    WNDCLASSEXW wcex{sizeof(WNDCLASSEX)};
    wcex.lpszClassName = className.c_str();
    wcex.lpszMenuName = className.c_str();
    wcex.lpfnWndProc = Window::WndProcCallback;
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = ::GetModuleHandleW(nullptr);
    wcex.hbrBackground = hBrush;
    wcex.hCursor = hCursor;
    wcex.hIcon = hIcon;
    wcex.hIconSm = hIcon;

    auto atom{::RegisterClassExW(&wcex)};

    if (atom == 0)
        ::MessageBoxW(nullptr, std::to_wstring(::GetLastError()).c_str(), L"Error", 0);

    ::CreateWindowExW(0, className.c_str(), name.c_str(), WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
                      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr,
                      ::GetModuleHandleW(nullptr), this);

    if (!m_hWnd)
        ::MessageBoxW(nullptr, std::to_wstring(::GetLastError()).c_str(), L"Error", 0);

    // glow::win32::set_darkmode(m_hwnd);
    // glow::win32::set_darktitle();
    // glow::win32::set_mica(m_hwnd);
    // glow::win32::window_cloak(m_hwnd);

    ::ShowWindow(m_hWnd, SW_SHOWDEFAULT);

    // glow::win32::window_uncloak(m_hwnd);

    // auto webview{glow::WebView("WebView", m_hwnd)};
}

Window::~Window() {}

HWND Window::get_hwnd() { return m_hWnd; }

LRESULT CALLBACK Window::WndProcCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Window* window = InstanceFromWndProc<Window, Window, &Window::m_hWnd>(hWnd, uMsg, lParam);

    if (window)
    {
        return window->WndProc(hWnd, uMsg, wParam, lParam);
    }

    return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

LRESULT Window::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Window* window = InstanceFromWndProc<Window, Window, &Window::m_hWnd>(hWnd, uMsg, lParam);

    if (window)
    {
        switch (uMsg)
        {
        case WM_CLOSE:
        {
            ::DestroyWindow(hWnd);
            return 0;
        }

        case WM_DESTROY:
        {
            ::PostQuitMessage(0);
            return 0;
        }

            // case WM_SIZE:
            // {
            //     RECT r;
            //     ::GetClientRect(hWnd, &r);
            //     ::EnumChildWindows(hWnd, EnumChildProcCallback, (LPARAM)&r);

            //     return 0;
            // }
        }
    }

    return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

// int __stdcall Window::EnumChildProcCallback(HWND hwnd, LPARAM lparam)
// {
//     auto child{GetWindowLongPtrW(hwnd, GWL_ID)};
//     auto p{(LPRECT)lparam};

//     SetWindowPos(hwnd, nullptr, 0, 0, p->right, p->bottom, SWP_NOZORDER);

//     return 1;
// }

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
