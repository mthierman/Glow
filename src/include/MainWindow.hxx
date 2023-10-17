#pragma once

#include <Windows.h>
#include <dwmapi.h>
#include <string>
#include "Helpers.hxx"

#define IDM_SETTINGS 1001

namespace glow
{
class MainWindow
{
  public:
    MainWindow(std::string);
    ~MainWindow();

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

MainWindow::MainWindow(std::string t)
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
    wcex.lpfnWndProc = MainWindow::WndProcCallback;
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

    auto hMenu{::GetSystemMenu(m_hWnd, FALSE)};

    MENUITEMINFOW settings{sizeof(MENUITEMINFOW)};
    settings.fMask = MIIM_STRING | MIIM_ID;
    settings.wID = IDM_SETTINGS;
    settings.dwTypeData = const_cast<LPWSTR>(L"Settings");

    MENUITEMINFOW seperator{sizeof(MENUITEMINFOW)};
    seperator.fMask = MIIM_FTYPE;
    seperator.fType = MFT_SEPARATOR;

    if (hMenu != INVALID_HANDLE_VALUE)
    {
        ::InsertMenuItemW(hMenu, 1, TRUE, &seperator);
        ::InsertMenuItemW(hMenu, 2, TRUE, &settings);
        ::InsertMenuItemW(hMenu, 3, TRUE, &seperator);
    }
}

MainWindow::~MainWindow() {}

HWND MainWindow::get_hwnd() { return m_hWnd; }

LRESULT CALLBACK MainWindow::WndProcCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    MainWindow* pMainWindow =
        InstanceFromWndProc<MainWindow, MainWindow, &MainWindow::m_hWnd>(hWnd, uMsg, lParam);

    if (pMainWindow)
        return pMainWindow->WndProc(hWnd, uMsg, wParam, lParam);

    return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

LRESULT MainWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    MainWindow* pMainWindow =
        InstanceFromWndProc<MainWindow, MainWindow, &MainWindow::m_hWnd>(hWnd, uMsg, lParam);

    if (pMainWindow)
    {
        switch (uMsg)
        {
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

int MainWindow::_OnClose(HWND hWnd)
{
    ::DestroyWindow(hWnd);

    return 0;
}

int MainWindow::_OnCreate(HWND hWnd)
{
    RECT r;
    GetWindowRect(hWnd, &r);

    return 0;
}

int MainWindow::_OnDestroy()
{
    ::PostQuitMessage(0);

    return 0;
}

int MainWindow::_OnSize(HWND hWnd)
{
    RECT r;
    ::GetClientRect(hWnd, &r);

    return 0;
}

int MainWindow::_OnSysCommand(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (wParam == IDM_SETTINGS)
        ::MessageBoxW(nullptr, std::to_wstring(::GetLastError()).c_str(), L"Error", 0);

    ::DefWindowProcW(hWnd, uMsg, wParam, lParam);

    return 0;
}
} // namespace glow
