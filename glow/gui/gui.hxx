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

template <typename T> T* InstanceFromWndProc(HWND hwnd, UINT uMsg, LPARAM lParam)
{
    T* pInstance{nullptr};

    if (uMsg == WM_NCCREATE)
    {
        auto lpCreateStruct{std::bit_cast<LPCREATESTRUCT>(lParam)};
        pInstance = static_cast<T*>(lpCreateStruct->lpCreateParams);
        pInstance->m_hwnd.reset(hwnd);
        SetWindowLongPtrA(hwnd, GWLP_USERDATA, std::bit_cast<LONG_PTR>(pInstance));
    }

    else pInstance = std::bit_cast<T*>(GetWindowLongPtrA(hwnd, GWLP_USERDATA));

    return pInstance;
}

template <typename T> T* NewInstanceFromWndProc(HWND hWnd, UINT uMsg, LPARAM lParam)
{
    T* self{nullptr};

    if (uMsg == WM_NCCREATE)
    {
        auto lpCreateStruct{std::bit_cast<LPCREATESTRUCT>(lParam)};
        self = static_cast<T*>(lpCreateStruct->lpCreateParams);
        self->m_hwnd.reset(hWnd);
        SetWindowLongPtrA(hWnd, 0, reinterpret_cast<LONG_PTR>(self));
    }

    else self = std::bit_cast<T*>(GetWindowLongPtrA(hWnd, 0));

    return self;
}

// auto register_window() -> ATOM;
// auto create_window(ATOM atom) -> HWND;
// auto show_window(HWND hwnd) -> void;
auto message_loop() -> void;

auto check_theme() -> bool;
auto set_darkmode(HWND hwnd) -> bool;
auto set_darktitle() -> bool;
auto window_cloak(HWND hwnd) -> bool;
auto window_uncloak(HWND hwnd) -> bool;
auto window_mica(HWND hwnd) -> bool;
auto window_maximize(HWND hwnd) -> bool;
auto window_fullscreen(HWND hwnd) -> bool;
auto window_topmost(HWND hwnd) -> bool;

} // namespace glow::gui
