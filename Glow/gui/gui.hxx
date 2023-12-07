// clang-format off
// ╔──────────────────────╗
// │                      │
// │     ╔═╗╦  ╔═╗╦ ╦     │  Glow - https://github.com/mthierman/Glow
// │     ║ ╦║  ║ ║║║║     │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │     ╚═╝╩═╝╚═╝╚╩╝     │  SPDX-License-Identifier: MIT
// │                      │
// ╚──────────────────────╝
// clang-format on

#pragma once

#include <Windows.h>
#include <dwmapi.h>
#include <ShlObj.h>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.ViewManagement.h>

//==============================================================================
namespace glow::gui
{
struct Bounds
{
    int x{0};
    int y{0};
    int width{0};
    int height{0};
};

template <class T, HWND(T::*m_hwnd)> T* InstanceFromWndProc(HWND hwnd, UINT uMsg, LPARAM lParam)
{
    T* pInstance;

    if (uMsg == WM_NCCREATE)
    {
        LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
        pInstance = reinterpret_cast<T*>(pCreateStruct->lpCreateParams);
        pInstance->*m_hwnd = hwnd;
        ::SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pInstance));
    }

    else pInstance = reinterpret_cast<T*>(::GetWindowLongPtr(hwnd, GWLP_USERDATA));

    return pInstance;
}

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
