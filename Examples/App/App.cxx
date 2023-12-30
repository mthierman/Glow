// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include <config/glow.hxx>

#include <gui/mainwindow.hxx>
#include <gui/webview.hxx>

struct App final : public glow::gui::MainWindow
{
    using glow::gui::MainWindow::MainWindow;
    auto handle_enum_child_proc(HWND hWnd, LPARAM lParam) -> BOOL override;
    auto handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT override;
    auto on_parent_notify(WPARAM wParam) -> int;
    auto on_size() -> int;
};

auto App::handle_enum_child_proc(HWND hWnd, LPARAM lParam) -> BOOL
{
    auto gwlId{GetWindowLongPtrA(hWnd, GWL_ID)};
    auto rectParent{*std::bit_cast<LPRECT>(lParam)};
    auto position{glow::gui::rect_to_position(rectParent)};

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
    // OutputDebugStringA("WM_PARENTNOTIFY\n");
    // OutputDebugStringA(std::to_string(LOWORD(wParam)).c_str());
    // OutputDebugStringA("\n");

    on_size();

    return 0;
}

auto App::on_size() -> int
{
    // OutputDebugStringA("WM_SIZE\n");

    RECT clientRect{0};
    GetClientRect(m_hwnd.get(), &clientRect);
    EnumChildWindows(m_hwnd.get(), EnumChildProc, std::bit_cast<LPARAM>(&clientRect));

    return 0;
}

auto main() -> int
{
    glow::gui::CoInitialize init;

    if (SUCCEEDED(init))
    {
        App app;
        // app.show_normal();
        glow::gui::WebView2 webView{app.m_hwnd.get(), 1};

        // app.show_normal();
        // webView.show_normal();

        glow::gui::message_loop();
    }

    return 0;
}
