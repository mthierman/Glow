// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#include <bit>

#include <gui/window.hxx>

namespace glow
{

struct Window : glow::gui::Window
{
    using glow::gui::Window::Window;

  private:
    auto handle_message(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT override;
    static auto enum_child_proc(HWND hwnd, LPARAM lParam) -> BOOL;

    auto on_notify() -> int;
    auto on_size() -> int;
};

auto Window::handle_message(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_NOTIFY: return on_notify();
    case WM_SIZE: return on_size();
    }

    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

auto CALLBACK Window::enum_child_proc(HWND hwnd, LPARAM lParam) -> BOOL
{
    auto childId{GetWindowLongPtrA(hwnd, GWL_ID)};

    auto rcParent{(LPRECT)lParam};

    if (childId == 1)
        SetWindowPos(hwnd, nullptr, 0, 0, (rcParent->right - rcParent->left),
                     (rcParent->bottom - rcParent->top), SWP_NOZORDER);

    return 1;
}

auto Window::on_notify() -> int
{
    on_size();

    return 0;
}

auto Window::on_size() -> int
{
    RECT clientRect{0};
    GetClientRect(m_hwnd.get(), &clientRect);
    EnumChildWindows(m_hwnd.get(), enum_child_proc, std::bit_cast<LPARAM>(&clientRect));
    Sleep(1);

    return 0;
}

} // namespace glow
