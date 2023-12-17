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

Window::Window()
{
    auto atom{register_window()};
    m_hwnd.reset(create_window(atom));
    show_window(m_hwnd.get());
    SetWindowLongPtrA(m_hwnd.get(), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(Window::WndProc));
    SetWindowLongPtrA(m_hwnd.get(), 0, reinterpret_cast<LONG_PTR>(this));
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
