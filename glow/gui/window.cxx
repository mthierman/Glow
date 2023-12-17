// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include "window.hxx"

#include <gui/gui.hxx>

namespace glow::gui
{

// https://stackoverflow.com/questions/117792/best-method-for-storing-this-pointer-for-use-in-wndproc

// Window::Window()
// {
//     auto atom{register_window()};
//     m_hwnd.reset(create_window(atom));
//     show_window(m_hwnd.get());
//     SetWindowLongPtrA(m_hwnd.get(), 0, reinterpret_cast<LONG_PTR>(this));
// }

Window::Window()
{
    HCURSOR cursor{static_cast<HCURSOR>(
        LoadImageA(nullptr, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED | LR_DEFAULTSIZE))};

    HICON icon{static_cast<HICON>(
        LoadImageA(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE))};

    HBRUSH background{static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH))};

    WNDCLASSEX wcex{sizeof(WNDCLASSEX)};
    wcex.lpszClassName = "TEST";
    wcex.lpszMenuName = "TEST";
    wcex.lpfnWndProc = DefWindowProcA;
    wcex.style = 0;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = sizeof(void*);
    wcex.hInstance = GetModuleHandleA(nullptr);
    wcex.hbrBackground = background;
    wcex.hCursor = cursor;
    wcex.hIcon = icon;
    wcex.hIconSm = icon;

    auto atom = RegisterClassExA(&wcex);

    m_hwnd.reset(CreateWindowExA(0, MAKEINTATOM(atom), "TEST",
                                 WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, CW_USEDEFAULT,
                                 CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr,
                                 GetModuleHandleA(nullptr), nullptr));

    SetWindowLongPtrA(m_hwnd.get(), 0, reinterpret_cast<LONG_PTR>(this));

    ShowWindow(m_hwnd.get(), SW_SHOWDEFAULT);
}

Window::~Window() {}

auto CALLBACK Window::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    // auto self{InstanceFromWndProc<Window>(hwnd, uMsg, lParam)};

    auto self = std::bit_cast<Window*>(GetWindowLongPtrA(hwnd, 0));

    if (self)
    {
        OutputDebugStringA("SELF EXISTS!");
        switch (uMsg)
        {
        case WM_SIZE: return self->on_size();
        case WM_CLOSE: return self->on_close();
        }

        return self->handle_message(hwnd, uMsg, wParam, lParam);
    }

    else return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

auto Window::handle_message(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    return DefWindowProcA(m_hwnd.get(), uMsg, wParam, lParam);
}

auto Window::on_close() -> int
{
    PostQuitMessage(0);
    return 0;
}

auto Window::on_size() -> int
{
    OutputDebugStringA("WM_SIZE\n");
    return 0;
}

} // namespace glow::gui
