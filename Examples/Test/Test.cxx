// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include <glow/config.hxx>

#include <glow/console.hxx>
#include <glow/filesystem.hxx>
#include <glow/gui.hxx>
#include <glow/log.hxx>
#include <glow/mainwindow.hxx>
#include <glow/text.hxx>
#include <glow/webview.hxx>
#include <glow/window.hxx>

struct App final : public glow::gui::MainWindow
{
    using glow::gui::MainWindow::MainWindow;

    static auto run() -> int;

    static auto CALLBACK EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL;

    auto handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT override;
    auto on_parent_notify(WPARAM wParam) -> int;
    auto on_size() -> int;

    glow::gui::GdiPlus gdiInit;
    glow::gui::CoInitialize coInit;
};

auto App::run() -> int
{
    glow::console::Console console;

    std::cout << "Test" << std::endl;

    App app;
    glow::gui::WebView2 webView{app.m_hwnd.get(), 1};

    webView.show_normal();

    app.show_normal();

    return glow::gui::message_loop();
}

auto CALLBACK App::EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL
{
    auto gwlId{GetWindowLongPtrA(hWnd, GWL_ID)};

    auto rect{*std::bit_cast<LPRECT>(lParam)};
    auto position{glow::gui::rect_to_position(rect)};

    if (gwlId == 1)
        SetWindowPos(hWnd, nullptr, 0, 0, position.width, position.height, SWP_NOZORDER);

    return TRUE;
}

auto App::handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_PARENTNOTIFY: return on_parent_notify(wParam);
    case WM_SIZE: return on_size();
    }

    return DefWindowProcA(m_hwnd.get(), uMsg, wParam, lParam);
}

auto App::on_parent_notify(WPARAM wParam) -> int
{
    if (LOWORD(wParam) == WM_CREATE) on_size();

    return 0;
}

auto App::on_size() -> int
{
    RECT rect{0};
    GetClientRect(m_hwnd.get(), &rect);
    EnumChildWindows(m_hwnd.get(), EnumChildProc, std::bit_cast<LPARAM>(&rect));
    Sleep(1);

    return 0;
}

auto main() -> int
{
    try
    {
        return App::run();
    }
    catch (std::exception& e)
    {
        glow::log::shell(e.what());
        std::terminate();
    }
}
