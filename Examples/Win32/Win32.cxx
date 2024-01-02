// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include <type_traits>

#include <glow/config.hxx>

#include <glow/console.hxx>
#include <glow/filesystem.hxx>
#include <glow/text.hxx>
#include <glow/webview.hxx>
#include <glow/window.hxx>

using namespace glow;

struct App final : public window::MainWindow
{
    using MainWindow::MainWindow;

    static auto run() -> int;

    static auto CALLBACK EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL;

    auto handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT override;
    auto on_parent_notify(WPARAM wParam) -> int;
    auto on_size() -> int;

    window::GdiPlus gdiInit;
    window::CoInitialize coInit;
};

auto App::run() -> int
{
    App app{"Win32"};
    app.create();
    app.show_normal();

    window::WebView webView{1, app.m_hwnd.get(), "https://www.bing.com/"};
    webView.create();
    webView.show_normal();
    glow::console::debug(std::to_string(webView.m_id));

    return window::message_loop();
}

auto CALLBACK App::EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL
{
    auto gwlId{GetWindowLongPtrA(hWnd, GWL_ID)};

    auto rect{*std::bit_cast<LPRECT>(lParam)};
    auto position{window::rect_to_position(rect)};

    if (gwlId == 1)
    {
        SetWindowPos(hWnd, nullptr, 0, 0, position.width, position.height, SWP_NOZORDER);
    }

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
        glow::console::debug(e.what());
        std::terminate();
    }
}
