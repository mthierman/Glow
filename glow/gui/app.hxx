// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#include <Windows.h>

#include <string>
#include <string_view>

#include <wil/resource.h>

#include <gui/gui.hxx>
#include <text/text.hxx>

namespace glow::gui
{

struct App
{
    App(std::string_view name);
    virtual ~App();

    auto register_window() -> ATOM;
    auto create_window() -> void;

    auto show_window_default() -> void;
    auto show_window() -> void;
    auto hide_window() -> void;

    static auto CALLBACK wnd_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    virtual auto handle_message(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;

    virtual auto on_close() -> int;
    virtual auto on_destroy() -> int;

    wil::unique_hwnd m_hwnd;
    std::string m_name;
    std::string m_class;
    ATOM m_classAtom{};

    HCURSOR m_cursor{static_cast<HCURSOR>(
        LoadImageA(nullptr, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED | LR_DEFAULTSIZE))};

    HICON m_defaultIcon{static_cast<HICON>(
        LoadImageA(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE))};

    wil::unique_hicon m_icon{static_cast<HICON>(LoadImageA(
        GetModuleHandleA(nullptr), MAKEINTRESOURCEA(101), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE))};

    HBRUSH m_background{static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH))};
};

} // namespace glow::gui
