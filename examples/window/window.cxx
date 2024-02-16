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
    auto on_close(::WPARAM wParam, ::LPARAM lParam) -> int override;
    auto on_create(::WPARAM wParam, ::LPARAM lParam) -> int override;
    auto on_destroy(::WPARAM wParam, ::LPARAM lParam) -> int;
};

auto Window::WndProc(::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam) -> ::LRESULT
{
    // glow::log("wnd_proc [derived]\n");
    // glow::log(std::to_string(uMsg));

    // switch (uMsg)
    // {
    //     case WM_ACTIVATE: return 0;
    //     // case WM_CREATE: return on_create(wParam, lParam);
    //     // case WM_CLOSE: return on_close(wParam, lParam);
    //     // case WM_DESTROY: return on_destroy(wParam, lParam);
    //     default: return ::DefWindowProcA(m_hwnd.get(), uMsg, wParam, lParam);
    // }

    return ::DefWindowProcA(m_hwnd.get(), uMsg, wParam, lParam);
}

auto Window::on_close(::WPARAM wParam, ::LPARAM lParam) -> int
{
    glow::log("WM_CLOSE [derived]\n");
    // m_hwnd.reset();

    return 0;
}

auto Window::on_create(::WPARAM wParam, ::LPARAM lParam) -> int
{
    glow::log("WM_CREATE [derived]\n");

    return 0;
}

auto Window::on_destroy(::WPARAM wParam, ::LPARAM lParam) -> int
{
    glow::log("WM_DESTROY [derived]\n");
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
