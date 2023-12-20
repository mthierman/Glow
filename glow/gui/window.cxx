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

Window::Window()
{
    create_window();
    glow::gui::window_cloak(m_hwnd.get());
    show_normal();
    glow::gui::window_uncloak(m_hwnd.get());
}

Window::Window(std::string title) : Window() { set_title(title); }

Window::~Window() {}

auto Window::register_class() -> void
{
    wcex.lpszClassName = "Window";
    wcex.lpszMenuName = 0;
    wcex.lpfnWndProc = Window::WndProc;
    wcex.style = 0;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = sizeof(void*);
    wcex.hInstance = GetModuleHandleA(nullptr);
    wcex.hbrBackground = m_hbrBackground.get();
    wcex.hCursor = m_hCursor.get();
    wcex.hIcon = m_appIcon.get() ? m_appIcon.get() : m_hIcon.get();
    wcex.hIconSm = m_appIcon.get() ? m_appIcon.get() : m_hIcon.get();

    RegisterClassExA(&wcex);
}

auto Window::create_window() -> void
{
    auto classInfo{GetClassInfoExA(GetModuleHandleA(nullptr), "Window", &wcex)};
    if (!classInfo)
    {
        OutputDebugStringA("Registering class...");
        register_class();
    }

    CreateWindowExA(0, "Window", "Window", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, CW_USEDEFAULT,
                    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr,
                    GetModuleHandleA(nullptr), this);
}

auto Window::show_normal() -> void { ShowWindow(m_hwnd.get(), SW_SHOWNORMAL); }

auto Window::show() -> void { ShowWindow(m_hwnd.get(), SW_SHOW); }

auto Window::hide() -> void { ShowWindow(m_hwnd.get(), SW_HIDE); }

auto Window::set_title(std::string title) -> void { SetWindowTextA(m_hwnd.get(), title.c_str()); }

auto Window::set_border(bool enabled) -> void
{
    auto style{GetWindowLongPtrA(m_hwnd.get(), GWL_STYLE)};

    SetWindowLongPtrA(m_hwnd.get(), GWL_STYLE,
                      enabled ? (style | WS_BORDER) : (style & ~WS_BORDER));
    SetWindowPos(m_hwnd.get(), nullptr, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
}

auto Window::set_child() -> void
{
    SetWindowLongPtrA(m_hwnd.get(), GWL_STYLE, WS_CHILD);
    SetWindowPos(m_hwnd.get(), nullptr, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
}

auto Window::set_popup() -> void
{
    SetWindowLongPtrA(m_hwnd.get(), GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN);
    SetWindowPos(m_hwnd.get(), nullptr, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
}

auto Window::set_overlapped() -> void
{
    SetWindowLongPtrA(m_hwnd.get(), GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN);
    SetWindowPos(m_hwnd.get(), nullptr, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
}

auto CALLBACK Window::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    auto self{InstanceFromWndProc<Window>(hWnd, uMsg, lParam)};

    if (self)
    {
        switch (uMsg)
        {
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
    m_hwnd.reset();

    return 0;
}

} // namespace glow::gui
