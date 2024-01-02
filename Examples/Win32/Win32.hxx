// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#include <glow/config.hxx>

#include <glow/console.hxx>
#include <glow/filesystem.hxx>
#include <glow/text.hxx>
#include <glow/window.hxx>

namespace App
{
using namespace glow;

struct MainWindow final : public window::MainWindow
{
    using window::MainWindow::MainWindow;

    static auto CALLBACK EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL;

    auto handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT override;
    auto on_parent_notify(WPARAM wParam) -> int;
    auto on_size() -> int;

    window::GdiPlus gdiInit;
    window::CoInitialize coInit;

    std::unique_ptr<window::WebView> m_webView;
    std::unique_ptr<window::Window> m_window;
};

auto CALLBACK MainWindow::EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL
{
    auto gwlId{GetWindowLongPtrA(hWnd, GWL_ID)};

    auto rect{*std::bit_cast<LPRECT>(lParam)};
    auto position{window::rect_to_position(rect)};

    if (gwlId == 1)
        SetWindowPos(hWnd, nullptr, 0, 0, position.width, position.height, SWP_NOZORDER);

    return TRUE;
}

auto MainWindow::handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_PARENTNOTIFY: return on_parent_notify(wParam);
    case WM_SIZE: return on_size();
    }

    return DefWindowProcA(m_hwnd.get(), uMsg, wParam, lParam);
}

auto MainWindow::on_parent_notify(WPARAM wParam) -> int
{
    if (LOWORD(wParam) == WM_CREATE) on_size();

    return 0;
}

auto MainWindow::on_size() -> int
{
    RECT rect{0};
    GetClientRect(m_hwnd.get(), &rect);
    EnumChildWindows(m_hwnd.get(), EnumChildProc, std::bit_cast<LPARAM>(&rect));
    Sleep(1);

    return 0;
}

auto run() -> int
{
    MainWindow mainWindow{"Win32"};
    mainWindow(true);

    mainWindow.m_webView = std::make_unique<window::WebView>(1, mainWindow.m_hwnd.get());
    mainWindow.m_window = std::make_unique<window::Window>();

    (*mainWindow.m_webView)(true);
    mainWindow.m_window->operator()(true);

    return window::message_loop();
}
} // namespace App
