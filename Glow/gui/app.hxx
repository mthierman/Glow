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

#include <print>
#include <string>

#include <gui/gui.hxx>
#include <text/text.hxx>

//==============================================================================
namespace glow::gui
{
struct App
{
    App(std::string name);
    virtual ~App();

    //==============================================================================
    auto get_hwnd() -> HWND;

  private:
    auto register_window() -> void;
    auto create_window(std::string name) -> void;

    //==============================================================================
    auto show_window_default() -> void;
    auto show_window() -> void;
    auto hide_window() -> void;

    //==============================================================================
    static auto CALLBACK wnd_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    virtual auto handle_message(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;

    //==============================================================================
    auto on_close() -> int;
    auto on_destroy() -> int;

    //==============================================================================
    HWND m_hwnd{nullptr};
    std::string m_class;
    HCURSOR m_cursor{
        reinterpret_cast<HCURSOR>(::LoadImage(nullptr, reinterpret_cast<LPCSTR>(IDC_ARROW),
                                              IMAGE_CURSOR, 0, 0, LR_SHARED | LR_DEFAULTSIZE))};
    HICON m_defaultIcon{
        reinterpret_cast<HICON>(::LoadImage(nullptr, reinterpret_cast<LPCSTR>(IDI_APPLICATION),
                                            IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE))};
    HICON m_icon{reinterpret_cast<HICON>(::LoadImage(::GetModuleHandle(nullptr), MAKEINTRESOURCE(1),
                                                     IMAGE_ICON, 0, 0, LR_DEFAULTSIZE))};
    HBRUSH m_background{reinterpret_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH))};
};
} // namespace glow::gui
