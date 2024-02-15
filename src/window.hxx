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

#ifdef small
#undef small
#endif

#include <stdexcept>
#include <string>

#include <wil/resource.h>

#include "gui.hxx"
#include "notification.hxx"
#include "position.hxx"
#include "random.hxx"

namespace glow
{
struct Window
{
    Window(std::string name = "Window", size_t id = glow::random<size_t>(),
           ::DWORD style = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, ::DWORD exStyle = 0,
           int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int width = CW_USEDEFAULT,
           int height = CW_USEDEFAULT, ::HWND parent = nullptr, ::HMENU menu = nullptr);
    virtual ~Window();

    virtual auto notify(::HWND receiver, CODE code, std::string message = "") -> void;

    virtual auto get_dpi() -> unsigned int;
    virtual auto get_scale() -> float;
    virtual auto get_style() -> intptr_t;
    virtual auto get_ex_style() -> intptr_t;
    virtual auto get_id() -> intptr_t;
    virtual auto get_parent() -> ::HWND;
    virtual auto get_placement() -> ::WINDOWPLACEMENT;

    virtual auto reveal() -> bool;
    virtual auto show() -> bool;
    virtual auto hide() -> bool;
    virtual auto maximize() -> bool;
    virtual auto restore() -> bool;
    virtual auto close() -> void;

    virtual auto is_visible() -> bool;
    virtual auto focus() -> ::HWND;
    virtual auto is_focus() -> bool;
    virtual auto foreground() -> bool;
    virtual auto is_foreground() -> bool;
    virtual auto active() -> bool;
    virtual auto top() -> bool;
    virtual auto toggle_maximize() -> void;
    virtual auto toggle_fullscreen() -> void;
    virtual auto toggle_topmost() -> void;
    virtual auto set_title(std::string title) -> bool;
    virtual auto set_icon(::HICON icon, bool small = true, bool big = true) -> void;
    virtual auto border(bool enabled) -> void;
    virtual auto set_overlapped() -> void;
    virtual auto set_popup() -> void;
    virtual auto set_child() -> void;
    virtual auto reparent(::HWND parent) -> void;
    virtual auto position() -> void;
    virtual auto resize() -> void;
    virtual auto theme() -> void;
    virtual auto dwm_dark_mode(bool enabled) -> void;
    virtual auto dwm_system_backdrop(DWM_SYSTEMBACKDROP_TYPE backdrop) -> void;
    virtual auto dwm_rounded_corners(bool enable) -> void;
    virtual auto dwm_cloak(bool enable) -> void;
    virtual auto dwm_caption_color(bool enable) -> void;
    virtual auto dwm_set_caption_color(::COLORREF color) -> void;
    virtual auto dwm_border_color(bool enable) -> void;
    virtual auto dwm_set_border_color(::COLORREF color) -> void;
    virtual auto dwm_set_text_color(::COLORREF color) -> void;
    virtual auto dwm_reset_text_color() -> void;

    size_t m_id{};
    wil::unique_hwnd m_hwnd{};
    wil::unique_hicon m_appIcon{};
    wil::unique_hicon m_hicon{};
    glow::Position m_client{};
    glow::Position m_window{};
    unsigned int m_dpi{};
    float m_scale{};
    bool m_maximize{};
    bool m_fullscreen{};
    bool m_topmost{};
    glow::Notification m_notification{};

  private:
    static auto CALLBACK WndProc(::HWND hWnd, ::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam)
        -> ::LRESULT;
    virtual auto default_wnd_proc(::HWND hWnd, ::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam)
        -> ::LRESULT;
    virtual auto wnd_proc(::HWND hWnd, ::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam) -> ::LRESULT;
    virtual auto on_close(::WPARAM wParam, ::LPARAM lParam) -> int;
    virtual auto on_create(::WPARAM wParam, ::LPARAM lParam) -> int;
    virtual auto on_dpi_changed(::WPARAM wParam, ::LPARAM lParam) -> int;
};
} // namespace glow
