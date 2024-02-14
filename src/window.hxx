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
    virtual auto dpi() -> void;
    virtual auto scale() -> void;
    virtual auto reveal() -> void;
    virtual auto show() -> void;
    virtual auto hide() -> void;
    virtual auto is_visible() -> bool;
    virtual auto focus() -> void;
    virtual auto is_focus() -> bool;
    virtual auto foreground() -> void;
    virtual auto is_foreground() -> bool;
    virtual auto active() -> void;
    virtual auto top() -> void;
    virtual auto maximize() -> void;
    virtual auto fullscreen() -> void;
    virtual auto topmost() -> void;
    virtual auto title(std::string title) -> void;
    virtual auto icon(::HICON icon, bool small = true, bool big = true) -> void;
    virtual auto border(bool enabled) -> void;
    virtual auto overlapped() -> void;
    virtual auto popup() -> void;
    virtual auto child() -> void;
    virtual auto reparent(::HWND parent) -> void;
    virtual auto position() -> void;
    virtual auto size() -> void;
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
    int m_dpi{};
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
    virtual auto wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
};
} // namespace glow
