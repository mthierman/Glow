// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include <gui/app.hxx>

//==============================================================================
namespace glow::gui
{

//==============================================================================
App::App(std::string name) : m_name(name)
{
    m_classAtom = register_window();
    create_window();
    show_window_default();
}

//==============================================================================
App::~App() {}

//==============================================================================
auto App::get_hwnd() -> HWND { return m_hwnd; }

//==============================================================================
auto App::register_window() -> ATOM
{
    WNDCLASSEX wcex{sizeof(WNDCLASSEX)};
    wcex.lpszClassName = m_name.c_str();
    wcex.lpszMenuName = m_name.c_str();
    wcex.lpfnWndProc = App::wnd_proc;
    wcex.style = 0;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = ::GetModuleHandle(nullptr);
    wcex.hbrBackground = m_background;
    wcex.hCursor = m_cursor;
    wcex.hIcon = m_icon ? m_icon : m_defaultIcon;
    wcex.hIconSm = m_icon ? m_icon : m_defaultIcon;

    return ::RegisterClassEx(&wcex);
}

//==============================================================================
auto App::create_window() -> void
{
    ::CreateWindowEx(0, MAKEINTATOM(m_classAtom), m_name.c_str(),
                     WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT,
                     CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, ::GetModuleHandle(nullptr),
                     this);
}

//==============================================================================
auto App::show_window_default() -> void { ::ShowWindow(m_hwnd, SW_SHOWDEFAULT); }

//==============================================================================
auto App::show_window() -> void { ::ShowWindow(m_hwnd, SW_SHOW); }

//==============================================================================
auto App::hide_window() -> void { ::ShowWindow(m_hwnd, SW_HIDE); }

//==============================================================================
auto CALLBACK App::wnd_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    App* self = InstanceFromWndProc<App, &App::m_hwnd>(hwnd, uMsg, lParam);

    if (self)
    {
        switch (uMsg)
        {
        case WM_CLOSE: return self->on_close();
        case WM_DESTROY: return self->on_destroy();
        }

        return self->handle_message(hwnd, uMsg, wParam, lParam);
    }

    else return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//==============================================================================
auto App::handle_message(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    return ::DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

//==============================================================================
auto App::on_close() -> int
{
    ::DestroyWindow(m_hwnd);

    return 0;
}

//==============================================================================
auto App::on_destroy() -> int
{
    ::PostQuitMessage(0);

    return 0;
}

//==============================================================================
} // namespace glow::gui