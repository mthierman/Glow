#pragma once

#include <Windows.h>
#include <dwmapi.h>
#include <string>

#define IDM_SETTINGS 1001

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

Window::Window(std::string t)
{
    auto name{glow::widen(t)};
    auto className{glow::widen("MainWindow")};
    auto hInstance = ::GetModuleHandleW(nullptr);

    auto hIcon{reinterpret_cast<HICON>(::LoadImageW(hInstance, glow::widen("APP_ICON").c_str(),
                                                    IMAGE_ICON, 0, 0, LR_DEFAULTSIZE))};

    auto hCursor{reinterpret_cast<HCURSOR>(::LoadImageW(
        nullptr, reinterpret_cast<LPCWSTR>(IDC_ARROW), IMAGE_CURSOR, 0, 0, LR_SHARED))};

    auto hBrush{reinterpret_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH))};

    WNDCLASSEXW wcex{sizeof(WNDCLASSEX)};
    wcex.lpszClassName = className.c_str();
    wcex.lpszMenuName = className.c_str();
    wcex.lpfnWndProc = Window::WndProcCallback;
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

    ::CreateWindowExW(0, className.c_str(), name.c_str(), WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
                      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr,
                      ::GetModuleHandleW(nullptr), this);

    if (!m_hWnd)
        ::MessageBoxW(nullptr, std::to_wstring(::GetLastError()).c_str(), L"Error", 0);

    ::ShowWindow(m_hWnd, SW_SHOWDEFAULT);

    auto hMenu{GetSystemMenu(m_hWnd, FALSE)};

    MENUITEMINFOW settings{sizeof(MENUITEMINFOW)};
    settings.fMask = MIIM_STRING | MIIM_ID;
    settings.wID = IDM_SETTINGS;
    settings.dwTypeData = const_cast<LPWSTR>(L"Settings");

    MENUITEMINFOW seperator{sizeof(MENUITEMINFOW)};
    seperator.fMask = MIIM_FTYPE;
    seperator.fType = MFT_SEPARATOR;

    if (hMenu != INVALID_HANDLE_VALUE)
    {
        InsertMenuItemW(hMenu, 1, TRUE, &seperator);
        InsertMenuItemW(hMenu, 2, TRUE, &settings);
        InsertMenuItemW(hMenu, 3, TRUE, &seperator);
        // AppendMenuW(hMenu, MF_SEPARATOR, 0, 0);
    }
}

Window::~Window() {}

HWND Window::get_hwnd() { return m_hWnd; }

LRESULT CALLBACK Window::WndProcCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Window* pMainWindow = InstanceFromWndProc<Window, Window, &Window::m_hWnd>(hWnd, uMsg, lParam);

    if (pMainWindow)
        return pMainWindow->WndProc(hWnd, uMsg, wParam, lParam);

    return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

LRESULT Window::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Window* pMainWindow = InstanceFromWndProc<Window, Window, &Window::m_hWnd>(hWnd, uMsg, lParam);

    if (pMainWindow)
    {
        switch (uMsg)
        {
        case WM_ACTIVATE:
            return pMainWindow->_OnActivate(hWnd);
        case WM_CLOSE:
            return pMainWindow->_OnClose(hWnd);
        case WM_CREATE:
            return pMainWindow->_OnCreate(hWnd);
        case WM_DESTROY:
            return pMainWindow->_OnDestroy();
        case WM_SIZE:
            return pMainWindow->_OnSize(hWnd);
        case WM_SYSCOMMAND:
            return pMainWindow->_OnSysCommand(hWnd, uMsg, wParam, lParam);
        }
    }

    return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

int Window::_OnActivate(HWND hWnd)
{
    MARGINS m{0, 0, 0, 0};

    if (!SUCCEEDED(DwmExtendFrameIntoClientArea(hWnd, &m)))
        ::MessageBoxW(nullptr, std::to_wstring(::GetLastError()).c_str(), L"Error", 0);

    return 0;
}

int Window::_OnClose(HWND hWnd)
{
    ::DestroyWindow(hWnd);
    return 0;
}

int Window::_OnCreate(HWND hWnd)
{
    RECT r;
    GetWindowRect(hWnd, &r);
    SetWindowPos(hWnd, 0, r.left, r.top, (r.right - r.left), (r.bottom - r.top), SWP_FRAMECHANGED);
    return 0;
}

int Window::_OnDestroy()
{
    ::PostQuitMessage(0);
    return 0;
}

int Window::_OnSize(HWND hWnd)
{
    RECT r;
    ::GetClientRect(hWnd, &r);

    return 0;
}

int Window::_OnSysCommand(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (wParam == IDM_SETTINGS)
        ::MessageBoxW(nullptr, std::to_wstring(::GetLastError()).c_str(), L"Error", 0);

    ::DefWindowProcW(hWnd, uMsg, wParam, lParam);

    return 0;
}
} // namespace glow
