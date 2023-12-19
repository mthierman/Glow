// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include <gui/window.hxx>

namespace glow::gui
{

// https://stackoverflow.com/questions/117792/best-method-for-storing-this-pointer-for-use-in-wndproc
Window::Window() { create(); }

Window::Window(std::string title) : m_title{title} { create(); }

Window::~Window() {}

auto Window::set_title(std::string title) -> void { SetWindowTextA(m_hwnd.get(), title.c_str()); }

auto Window::set_popup() -> void
{
    glow::gui::window_cloak(m_hwnd.get());
    auto style{GetWindowLongPtrA(m_hwnd.get(), GWL_STYLE)};
    SetWindowLongPtrA(m_hwnd.get(), GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW | WS_POPUP);
    SetWindowPos(m_hwnd.get(), nullptr, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
    glow::gui::window_uncloak(m_hwnd.get());
}

auto Window::set_overlapped() -> void
{
    glow::gui::window_cloak(m_hwnd.get());
    auto style{GetWindowLongPtrA(m_hwnd.get(), GWL_STYLE)};
    SetWindowLongPtrA(m_hwnd.get(), GWL_STYLE, style & ~WS_POPUP | WS_OVERLAPPEDWINDOW);
    SetWindowPos(m_hwnd.get(), nullptr, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
    glow::gui::window_uncloak(m_hwnd.get());
}

auto Window::show() -> void { ShowWindow(m_hwnd.get(), SW_SHOW); }

auto Window::hide() -> void { ShowWindow(m_hwnd.get(), SW_HIDE); }

auto Window::register_class() -> ATOM
{
    wcex.lpszClassName = m_title.c_str();
    wcex.lpszMenuName = m_title.c_str();
    wcex.lpfnWndProc = Window::WndProc;
    wcex.style = 0;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = sizeof(void*);
    wcex.hInstance = GetModuleHandleA(nullptr);
    wcex.hbrBackground = m_hbrBackground.get();
    wcex.hCursor = m_hCursor.get();
    wcex.hIcon = m_appIcon.get() ? m_appIcon.get() : m_hIcon.get();
    wcex.hIconSm = m_appIcon.get() ? m_appIcon.get() : m_hIcon.get();

    return RegisterClassExA(&wcex);
}

auto Window::create_window() -> HWND
{
    return CreateWindowExA(0, MAKEINTATOM(m_atom), m_title.c_str(),
                           WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT,
                           CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr,
                           GetModuleHandleA(nullptr), this);
}

auto Window::create() -> void
{
    m_atom = register_class();
    create_window();
    glow::gui::window_cloak(m_hwnd.get());
    show_normal();
    glow::gui::window_uncloak(m_hwnd.get());
}

auto Window::show_normal() -> void { ShowWindow(m_hwnd.get(), SW_SHOWNORMAL); }

auto CALLBACK Window::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    auto self{InstanceFromWndProc<Window>(hWnd, uMsg, lParam)};
    OutputDebugString(std::to_string(uMsg).c_str());
    OutputDebugString("\n");

    if (self)
    {
        switch (uMsg)
        {
        case WM_CLOSE: return self->on_close();
        case WM_DESTROY: return self->on_destroy();
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
    m_hwnd.reset();

    return 0;
}

auto Window::on_destroy() -> int
{
    PostQuitMessage(0);

    return 0;
}

} // namespace glow::gui
