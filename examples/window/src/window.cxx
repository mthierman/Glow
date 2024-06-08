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
    auto on_window_pos_changed(::WPARAM wParam, ::LPARAM lParam) -> int;
};

auto Window::WndProc(::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam) -> ::LRESULT
{
    switch (uMsg)
    {
        case WM_DESTROY:
            return on_destroy(wParam, lParam);
        case WM_WINDOWPOSCHANGED:
            return on_window_pos_changed(wParam, lParam);
    }

    return ::DefWindowProcA(m_hwnd.get(), uMsg, wParam, lParam);
}

auto Window::on_destroy(::WPARAM wParam, ::LPARAM lParam) -> int
{
    ::PostQuitMessage(0);

    return 0;
}

auto Window::on_window_pos_changed(::WPARAM wParam, ::LPARAM lParam) -> int
{
    glow::log("on_window_pos_changed");

    auto pos{reinterpret_cast<::WINDOWPOS*>(lParam)};

    ::RECT client{};
    ::RECT window{};
    GetWindowRect(m_hwnd.get(), &window);
    GetClientRect(m_hwnd.get(), &client);

    ::WINDOWPLACEMENT placement{sizeof(::WINDOWPLACEMENT)};
    GetWindowPlacement(m_hwnd.get(), &placement);

    // glow::log(std::to_string(pos->cx));
    // glow::log(std::to_string(pos->x));
    // glow::log(std::to_string(pos->y));
    // glow::log(std::to_string(pos->flags));
    // glow::log(std::to_string(client.right - client.left));
    // glow::log(std::to_string(window.right - window.left));
    // glow::log(std::to_string(placement.rcNormalPosition.right -
    // placement.rcNormalPosition.left));
    glow::log(std::to_string(placement.showCmd));
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

    // win.toggle_fullscreen();

    return app();
}
