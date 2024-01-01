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
    WNDCLASSEXA wcex{sizeof(WNDCLASSEXA)};
    auto classInfo{GetClassInfoExA(GetModuleHandleA(nullptr), MAKEINTATOM(Window::m_atom), &wcex)};

    if (!classInfo)
    {
        OutputDebugStringA("Registering Window class...");

        wcex.lpszClassName = "MainWindow";
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

        Window::m_atom = RegisterClassExA(&wcex);
    }

    CreateWindowExA(0, MAKEINTATOM(Window::m_atom), "Window", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
                    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr,
                    GetModuleHandleA(nullptr), this);
}

Window::~Window() {}

auto Window::show_normal() -> void { glow::gui::show_normal(m_hwnd.get()); }

auto Window::show() -> void { glow::gui::show(m_hwnd.get()); }

auto Window::hide() -> void { glow::gui::hide(m_hwnd.get()); }

auto CALLBACK Window::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    auto self{InstanceFromWndProc<Window>(hWnd, uMsg, lParam)};

    if (self)
    {
        switch (uMsg)
        {
        case WM_CLOSE: return self->on_close(hWnd, uMsg, wParam, lParam);
        case WM_DESTROY: return self->on_destroy(hWnd, uMsg, wParam, lParam);
        }

        return self->handle_wnd_proc(hWnd, uMsg, wParam, lParam);
    }

    else return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

auto Window::handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

auto Window::on_close(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> int
{
    m_hwnd.reset();

    return 0;
}

auto Window::on_destroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> int { return 0; }

} // namespace glow::gui
