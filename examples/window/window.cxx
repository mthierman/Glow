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
    auto WndProc(::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam) -> ::LRESULT override;
    auto on_destroy(::WPARAM wParam, ::LPARAM lParam) -> int;
};

auto Window::WndProc(::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam) -> ::LRESULT
{
    switch (uMsg)
    {
        case WM_DESTROY: return on_destroy(wParam, lParam);
    }

    return ::DefWindowProcA(m_hwnd.get(), uMsg, wParam, lParam);
}

auto Window::on_destroy(::WPARAM wParam, ::LPARAM lParam) -> int
{
    ::PostQuitMessage(0);

    return 0;
}

auto main(int argc, char* argv[]) -> int
{
    glow::App app;

    Window win;
    win.create_window();

    win.dwm_caption_color(false);
    win.dwm_system_backdrop(DWMSBT_MAINWINDOW);
    win.theme();

    win.reveal();

    return app();
}
