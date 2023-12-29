// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#include <gui/mainwindow.hxx>

#include "Browser.hxx"

using namespace glow::gui;

struct App final : public MainWindow
{
    using MainWindow::MainWindow;

    static auto enum_child_proc(HWND hwnd, LPARAM lParam) -> BOOL;

    auto handle_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT override;
    auto on_size() -> int;

    Browser browser{"https://www.google.com/", m_hwnd.get(), 1};
    // glow::gui::WebView2 wv{m_hwnd.get(), 1};
};

auto CALLBACK App::enum_child_proc(HWND hwnd, LPARAM lParam) -> BOOL
{
    auto childId{GetWindowLongPtrA(hwnd, GWL_ID)};
    auto rcParent{(LPRECT)lParam};

    if (childId == 1)
        SetWindowPos(hwnd, nullptr, 0, 0, (rcParent->right - rcParent->left) / 2,
                     (rcParent->bottom - rcParent->top), SWP_NOZORDER);

    if (childId == 2)
        SetWindowPos(hwnd, nullptr, (rcParent->right - rcParent->left) / 2, 0,
                     (rcParent->right - rcParent->left) / 2, (rcParent->bottom - rcParent->top),
                     SWP_NOZORDER);

    return 1;
}

auto App::handle_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_SIZE: return on_size();
    }

    return DefWindowProcA(m_hwnd.get(), uMsg, wParam, lParam);
}

auto App::on_size() -> int
{
    RECT clientRect{0};
    GetClientRect(m_hwnd.get(), &clientRect);
    EnumChildWindows(m_hwnd.get(), enum_child_proc, std::bit_cast<LPARAM>(&clientRect));
    Sleep(1);

    return 0;
}
