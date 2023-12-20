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

Window::Window() { create(); }

Window::~Window() {}

auto Window::create() -> void
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

    glow::gui::show_normal(m_hwnd.get());
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
