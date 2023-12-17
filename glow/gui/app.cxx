// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include "app.hxx"

namespace glow::gui
{

App::App(std::string_view name) : m_name(name), m_class(glow::text::randomize(name.data()))
{
    m_classAtom = register_window();
    create_window();
    show_window_default();
}

App::~App() {}

auto App::register_window() -> ATOM
{
    WNDCLASSEX wcex{sizeof(WNDCLASSEX)};
    wcex.lpszClassName = m_class.c_str();
    wcex.lpszMenuName = m_class.c_str();
    wcex.lpfnWndProc = App::wnd_proc;
    wcex.style = 0;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = GetModuleHandleA(nullptr);
    wcex.hbrBackground = m_background;
    wcex.hCursor = m_cursor;
    wcex.hIcon = m_icon.get() ? m_icon.get() : m_defaultIcon;
    wcex.hIconSm = m_icon.get() ? m_icon.get() : m_defaultIcon;

    return RegisterClassExA(&wcex);
}

auto App::create_window() -> void
{
    CreateWindowExA(0, MAKEINTATOM(m_classAtom), m_name.c_str(),
                    WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT,
                    CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, GetModuleHandleA(nullptr),
                    this);

    // auto hmenu{CreateMenu()};
    // AppendMenuA(hmenu, MF_STRING, 0, "Hello World");
    // SetMenu(m_hwnd.get(), hmenu);
}

auto App::show_window_default() -> void { ShowWindow(m_hwnd.get(), SW_SHOWDEFAULT); }

auto App::show_window() -> void { ShowWindow(m_hwnd.get(), SW_SHOW); }

auto App::hide_window() -> void { ShowWindow(m_hwnd.get(), SW_HIDE); }

auto CALLBACK App::wnd_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    auto self{InstanceFromWndProc<App>(hwnd, uMsg, lParam)};

    if (self)
    {
        switch (uMsg)
        {
        case WM_CLOSE: return self->on_close();
        case WM_DESTROY: return self->on_destroy();
        }

        return self->handle_message(hwnd, uMsg, wParam, lParam);
    }

    else return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

auto App::handle_message(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    return DefWindowProcA(m_hwnd.get(), uMsg, wParam, lParam);
}

auto App::on_close() -> int
{
    m_hwnd.reset();

    return 0;
}

auto App::on_destroy() -> int
{
    PostQuitMessage(0);

    return 0;
}

} // namespace glow::gui
