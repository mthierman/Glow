// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#include <Windows.h>

namespace glow
{
auto set_preferred_app_mode() -> void;
auto allow_dark_mode(::HWND hWnd, bool enable) -> void;

template <typename T> T* instance_from_wnd_proc(HWND hWnd, UINT uMsg, LPARAM lParam)
{
    T* self{nullptr};

    if (uMsg == WM_NCCREATE)
    {
        auto lpCreateStruct{reinterpret_cast<::CREATESTRUCTA*>(lParam)};
        self = static_cast<T*>(lpCreateStruct->lpCreateParams);
        ::SetWindowLongPtrA(hWnd, 0, reinterpret_cast<intptr_t>(self));
        self->m_hwnd.reset(hWnd);
    }

    else { self = reinterpret_cast<T*>(::GetWindowLongPtrA(hWnd, 0)); }

    return self;
}

template <typename T> T* instance_from_window_long_ptr(::HWND hwnd)
{
    T* self{reinterpret_cast<T*>(::GetWindowLongPtrA(hwnd, 0))};

    return self;
}
} // namespace glow
