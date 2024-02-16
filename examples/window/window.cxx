// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include <glow/config.hxx>
#include <glow/glow.hxx>

struct Window final : public glow::Window
{
    Window();

    auto wnd_proc(::HWND hWnd, ::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam) -> ::LRESULT override;
    auto on_create(::WPARAM wParam, ::LPARAM lParam) -> int;
};

Window::Window() : glow::Window("Test")
{
    dwm_caption_color(false);
    dwm_system_backdrop(DWMSBT_MAINWINDOW);
    theme();
}

auto Window::wnd_proc(::HWND hWnd, ::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam) -> ::LRESULT
{
    switch (uMsg)
    {
        case WM_CREATE: return on_create(wParam, lParam);
        case WM_CLOSE: m_hwnd.reset(); return 0;
        case WM_DESTROY: ::PostQuitMessage(0); return 0;
    }

    return ::DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

auto Window::on_create(::WPARAM wParam, ::LPARAM lParam) -> int
{
    glow::log("WM_CREATE");

    return 0;
}

auto main(int argc, char* argv[]) -> int
{
    glow::App app;

    Window win;
    win.reveal();

    return app();
}
