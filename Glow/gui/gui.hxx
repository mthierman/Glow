// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#include <Windows.h>
#include <dwmapi.h>
#include <ShlObj.h>

#include <bit>
#include <memory>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.ViewManagement.h>

#include <wil/resource.h>

namespace glow::gui
{

struct Bounds
{
    int x{};
    int y{};
    int width{};
    int height{};
};

template <typename T> T* InstanceFromWndProc(HWND hWnd, UINT uMsg, LPARAM lParam)
{
    T* self{nullptr};

    if (uMsg == WM_NCCREATE)
    {
        auto lpCreateStruct{std::bit_cast<LPCREATESTRUCT>(lParam)};
        self = static_cast<T*>(lpCreateStruct->lpCreateParams);
        self->m_hwnd.reset(hWnd);
        SetWindowLongPtrA(hWnd, 0, std::bit_cast<LONG_PTR>(self));
    }

    else self = std::bit_cast<T*>(GetWindowLongPtrA(hWnd, 0));

    return self;
}

auto message_loop() -> void;

auto check_theme() -> bool;

auto set_darkmode(HWND hwnd) -> bool;
auto set_darktitle() -> bool;
auto allow_dark_mode(HWND hwnd, bool enable) -> void;

auto window_cloak(HWND hwnd) -> bool;
auto window_uncloak(HWND hwnd) -> bool;

auto set_caption_color(HWND hwnd, bool enabled) -> void;
auto set_system_backdrop(HWND hwnd, DWM_SYSTEMBACKDROP_TYPE backdropType) -> void;

auto window_maximize(HWND hwnd) -> bool;
auto window_fullscreen(HWND hwnd) -> bool;
auto window_topmost(HWND hwnd) -> bool;

auto show_normal(HWND hwnd) -> void;
auto show(HWND hwnd) -> void;
auto hide(HWND hwnd) -> void;

auto set_title(HWND hwnd, std::string title) -> void;
auto set_border(HWND hwnd, bool enabled) -> void;
auto set_child(HWND hwnd) -> void;
auto set_popup(HWND hwnd) -> void;
auto set_overlapped(HWND hwnd) -> void;

} // namespace glow::gui
