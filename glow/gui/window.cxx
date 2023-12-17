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
Window::Window()
{
    m_atom = register_class();
    create();
    show();

    // SetWindowLongPtrA(m_hwnd.get(), 0, reinterpret_cast<LONG_PTR>(this));
}

Window::~Window() {}

auto Window::register_class() -> ATOM
{
    HCURSOR hCursor{static_cast<HCURSOR>(
        LoadImageA(nullptr, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED | LR_DEFAULTSIZE))};

    HICON hIcon{static_cast<HICON>(
        LoadImageA(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE))};

    HBRUSH hbrBackground{static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH))};

    WNDCLASSEX wcex{sizeof(WNDCLASSEX)};
    wcex.lpszClassName = "TEST";
    wcex.lpszMenuName = "TEST";
    wcex.lpfnWndProc = Window::WndProc;
    wcex.style = 0;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = sizeof(void*);
    wcex.hInstance = GetModuleHandleA(nullptr);
    wcex.hbrBackground = hbrBackground;
    wcex.hCursor = hCursor;
    wcex.hIcon = hIcon;
    wcex.hIconSm = hIcon;

    return RegisterClassExA(&wcex);
}

auto Window::create() -> HWND
{
    return CreateWindowExA(0, MAKEINTATOM(m_atom), "TEST", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
                           CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr,
                           nullptr, GetModuleHandleA(nullptr), this);
}

auto Window::show() -> void { ShowWindow(m_hwnd.get(), SW_SHOWDEFAULT); }

auto CALLBACK Window::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    // Window* self{nullptr};

    // if (uMsg == WM_NCCREATE)
    // {
    //     auto lpCreateStruct{std::bit_cast<LPCREATESTRUCT>(lParam)};
    //     self = static_cast<Window*>(lpCreateStruct->lpCreateParams);
    //     SetWindowLongPtrA(hWnd, 0, std::bit_cast<LONG_PTR>(self));
    //     self->m_hwnd.reset(hWnd);
    // }

    // else self = std::bit_cast<Window*>(GetWindowLongPtrA(hWnd, 0));

    auto self{NewInstanceFromWndProc<Window>(hWnd, uMsg, lParam)};

    if (self)
    {
        switch (uMsg)
        {
        case WM_SIZE: return self->on_size();
        case WM_CLOSE: return self->on_close();
        }

        return self->handle_message(hWnd, uMsg, wParam, lParam);
    }

    else return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

auto Window::handle_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
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
