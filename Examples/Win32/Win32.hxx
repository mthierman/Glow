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
#include <glow/gui.hxx>
#include <glow/text.hxx>

namespace App
{
using namespace glow;

struct MainWindow final : public gui::MainWindow
{
    using gui::MainWindow::MainWindow;

    static auto CALLBACK EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL;

    auto handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT override;
    auto on_parent_notify(WPARAM wParam) -> int;
    auto on_size() -> int;

    gui::GdiPlus gdiInit;
    gui::CoInitialize coInit;

    std::unique_ptr<gui::WebView> m_webView;
    std::unique_ptr<gui::Window> m_window;
};

auto CALLBACK MainWindow::EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL
{
    auto gwlId{GetWindowLongPtrA(hWnd, GWL_ID)};

    auto rect{*std::bit_cast<LPRECT>(lParam)};
    auto position{gui::rect_to_position(rect)};

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
    mainWindow();

    mainWindow.m_webView = std::make_unique<gui::WebView>(1, mainWindow.m_hwnd.get());
    mainWindow.m_window = std::make_unique<gui::Window>();

    (*mainWindow.m_webView)(false);
    // (*mainWindow.m_window)();
    mainWindow.m_window->operator()();

    (*mainWindow.m_webView).show();

    return gui::message_loop();
}
} // namespace App
