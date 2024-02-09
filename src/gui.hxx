// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#include <Windows.h>
#include <Unknwn.h>
#include <dwmapi.h>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.ViewManagement.h>

#include "position.hxx"

namespace glow
{
auto get_class_info(::ATOM& atom, ::WNDCLASSEXA& wndClass) -> bool;
auto rect_to_position(const ::RECT& rect) -> Position;
auto position_to_rect(const Position& position) -> ::RECT;
auto clamp_color(int value) -> int;
auto make_colorref(int r, int g, int b) -> ::COLORREF;
auto load_icon(::LPSTR name) -> ::HICON;
auto check_theme() -> bool;
auto set_preferred_app_mode() -> void;
auto allow_dark_mode(::HWND hWnd, bool enable) -> void;

template <typename T> T* instance_from_wnd_proc(::HWND hwnd, ::UINT uMsg, ::LPARAM lParam)
{
    T* self{nullptr};

    if (uMsg == WM_NCCREATE)
    {
        auto lpCreateStruct{reinterpret_cast<::LPCREATESTRUCT>(lParam)};
        self = static_cast<T*>(lpCreateStruct->lpCreateParams);
        self->m_hwnd.reset(hwnd);
        ::SetWindowLongPtrA(hwnd, 0, reinterpret_cast<uintptr_t>(self));
    }

    else self = reinterpret_cast<T*>(::GetWindowLongPtrA(hwnd, 0));

    return self;
}

template <typename T> T* instance_from_window_long_ptr(::HWND hwnd)
{
    T* self{reinterpret_cast<T*>(::GetWindowLongPtrA(hwnd, 0))};

    return self;
}
} // namespace glow
