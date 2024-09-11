// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <glow/math.hxx>
#include <glow/message.hxx>
#include <glow/system.hxx>

#include <Windows.h>

#include <dwmapi.h>

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <wil/resource.h>

namespace glow {
struct Position {
    uint64_t x { 0 };
    uint64_t y { 0 };
    uint64_t width { 0 };
    uint64_t height { 0 };
};

struct Window {
    auto create(const std::string& title) -> void;

private:
    static auto CALLBACK procedure(::HWND hwnd,
                                   ::UINT msg,
                                   ::WPARAM wparam,
                                   ::LPARAM lparam) -> ::LRESULT;

public:
    auto activate() -> void;
    auto show() -> void;
    auto hide() -> void;
    auto maximize() -> void;
    auto minimize() -> void;
    auto restore() -> void;
    auto is_visible() -> bool;
    auto is_maximized() -> bool;

    auto set_title(const std::string& title) -> void;

    auto set_position(Position position) -> void;

    auto set_style(::LONG_PTR style) -> void;
    auto get_style() -> ::LONG_PTR;

    auto toggle_centered(bool centered) -> void;
    auto toggle_topmost(bool topmost) -> void;
    auto toggle_maximize() -> void;
    auto flash() -> void;

    auto timer_start(::UINT_PTR timerId, ::UINT intervalMs) -> bool;
    auto timer_stop(::UINT_PTR timerId) -> bool;

    struct Positions {
        Position window;
        Position client;
        Position monitor;
        Position restore;
    };

    Positions position;
    ::WINDOWPLACEMENT placement;
    uint64_t dpi;
    double scale;

    glow::message::MessageHandler message;
    wil::unique_hwnd hwnd;
};
} // namespace glow

namespace glow::window {
// auto register_class(::WNDCLASSEXA* windowClass) -> void;
// auto get_class_name(::HWND hwnd) -> std::string;

// auto find_message_only(const std::string& name = "MessageWindow") -> ::HWND;

// auto set_title(::HWND hwnd, std::string title) -> bool;

// auto activate(::HWND hwnd) -> bool;
// auto show(::HWND hwnd) -> bool;
// auto hide(::HWND hwnd) -> bool;
// auto maximize(::HWND hwnd) -> bool;
// auto minimize(::HWND hwnd) -> bool;
// auto restore(::HWND hwnd) -> bool;
// auto check_visibility(::HWND hwnd) -> bool;
// auto check_maximize(::HWND hwnd) -> bool;
// auto toggle_maximize(::HWND hwnd) -> bool;
// auto toggle_fullscreen(::HWND hwnd) -> void;
// auto set_position(::HWND hwnd, int x, int y, int cx, int cy) -> bool;

// auto top(::HWND hwnd) -> bool;
// auto bottom(::HWND hwnd) -> bool;
// auto topmost(::HWND hwnd) -> bool;
// auto no_topmost(::HWND hwnd) -> bool;
// auto check_topmost(::HWND hwnd) -> bool;
// auto toggle_topmost(::HWND hwnd) -> bool;

// auto flash(::HWND hwnd) -> bool;

// auto cloak(::HWND hwnd) -> void;
// auto uncloak(::HWND hwnd) -> void;
// auto check_cloak(::HWND hwnd) -> bool;

// auto enable_dark_mode(::HWND hwnd) -> void;
// auto disable_dark_mode(::HWND hwnd) -> void;

// auto set_backdrop(::HWND hwnd, ::DWM_SYSTEMBACKDROP_TYPE backdrop) -> void;
// auto set_round_corners(::HWND hwnd, ::DWM_WINDOW_CORNER_PREFERENCE corner) -> void;
// auto set_caption_color(::HWND hwnd, ::COLORREF color) -> void;
// auto set_border_color(::HWND hwnd, ::COLORREF color) -> void;
// auto set_text_color(::HWND hwnd, ::COLORREF color) -> void;

// auto set_focus(::HWND hwnd) -> ::HWND;
// auto get_focus() -> ::HWND;

// auto set_foreground(::HWND hwnd) -> bool;
// auto get_foreground() -> ::HWND;

// auto set_active(::HWND hwnd) -> ::HWND;
// auto get_active() -> ::HWND;

// auto set_parent(::HWND hwnd, ::HWND parent) -> ::HWND;
// auto get_parent(::HWND hwnd) -> ::HWND;

// auto bring_to_top(::HWND hwnd) -> bool;

// auto close(::HWND hwnd) -> ::LRESULT;
// auto destroy(::HWND hwnd) -> bool;

// auto get_current_dpi(::HWND hwnd) -> ::UINT;
// auto get_current_scale(::HWND hwnd) -> double;

// auto center(::HWND hwnd) -> void;

// auto set_placement(::HWND hwnd, const ::WINDOWPLACEMENT* windowPlacement) -> bool;
// auto get_placement(::HWND hwnd, ::WINDOWPLACEMENT* windowPlacement) -> bool;

// auto get_client_rect(::HWND hwnd) -> ::RECT;
// auto get_window_rect(::HWND hwnd) -> ::RECT;

// auto set_style(::HWND hwnd, ::DWORD style) -> ::DWORD;
// auto get_style(::HWND hwnd) -> ::DWORD;
// auto set_ex_style(::HWND hwnd, ::DWORD exStyle) -> ::DWORD;
// auto get_ex_style(::HWND hwnd) -> ::DWORD;
// auto get_id(::HWND hwnd) -> uintptr_t;

// auto set_overlapped(::HWND hwnd) -> void;
// auto check_overlapped(::HWND hwnd) -> bool;

// auto set_child(::HWND hwnd) -> void;
// auto check_child(::HWND hwnd) -> bool;

// auto set_popup(::HWND hwnd) -> void;
// auto check_popup(::HWND hwnd) -> bool;

// auto set_small_icon(::HWND hwnd, ::HICON hicon) -> void;
// auto set_big_icon(::HWND hwnd, ::HICON hicon) -> void;

// auto add_border(::HWND hwnd) -> void;
// auto remove_border(::HWND hwnd) -> void;

// auto check_dark() -> bool;

// auto def_window_proc(::HWND hwnd, ::UINT msg, ::WPARAM wparam, ::LPARAM lparam) -> ::LRESULT;
// auto CALLBACK window_proc(::HWND hwnd, ::UINT msg, ::WPARAM wparam, ::LPARAM lparam) ->
// ::LRESULT;

// struct Window {
//     auto create(const std::string& title = "", bool visible = true) -> void;
//     auto create(::HWND parent, const std::string& title = "", bool visible = true) -> void;
//     auto create_message_only() -> void;

//     auto close() -> void;

//     auto message(::UINT msg, std::function<::LRESULT(glow::message::wm message)> callback) ->
//     bool; auto message(::UINT msg) -> bool; auto message(glow::message::wm message) -> ::LRESULT;

//     auto notify(glow::message::notice notice,
//                 const std::string& message = "",
//                 const std::string& receiver = "MessageWindow") -> void;
//     auto
//     notify(::HWND receiver, glow::message::notice notice, const std::string& message = "") ->
//     void;

//     uintptr_t m_id { glow::math::make_random<uintptr_t>() };
//     ::WNDCLASSEXA m_windowClass {
//         .cbSize { sizeof(::WNDCLASSEXA) },
//         .style { 0 },
//         .lpfnWndProc { window_proc },
//         .cbClsExtra { 0 },
//         .cbWndExtra { sizeof(Window) },
//         .hInstance { glow::system::get_instance() },
//         .hIcon { glow::system::load_resource_icon() ? glow::system::load_resource_icon()
//                                                     : glow::system::load_system_icon() },
//         .hCursor { glow::system::load_system_cursor() },
//         .hbrBackground { glow::system::load_system_brush() },
//         .lpszMenuName { nullptr },
//         .lpszClassName { "Window" },
//         .hIconSm { glow::system::load_resource_icon() ? glow::system::load_resource_icon()
//                                                       : glow::system::load_system_icon() }
//     };
//     ::WINDOWPLACEMENT m_windowPlacement {
//         .length { sizeof(::WINDOWPLACEMENT) },
//         .flags { 0 },
//         .showCmd { 0 },
//         .ptMinPosition { ::POINT { .x { 0 }, .y { 0 } } },
//         .ptMaxPosition { ::POINT { .x { 0 }, .y { 0 } } },
//         .rcNormalPosition { ::RECT { .left { 0 }, .top { 0 }, .right { 0 }, .bottom { 0 } } }
//     };
//     ::UINT m_dpi { USER_DEFAULT_SCREEN_DPI };
//     double m_scale { 0.0 };
//     std::unordered_map<::UINT, std::function<::LRESULT(glow::message::wm message)>> m_map;
//     wil::unique_hwnd m_hwnd;
// };

template <typename T> struct WindowManager {
    auto add(std::unique_ptr<T> window) -> void {
        m_keys.push_back(window->m_id);
        m_map.try_emplace(window->m_id, std::move(window));
    }

    auto remove(uintptr_t id) -> void {
        auto pos { std::find(m_keys.begin(), m_keys.end(), id) };

        if (pos != m_keys.end()) {
            m_keys.erase(pos);
        }

        m_map.erase(id);

        if (m_map.empty()) {
            glow::system::quit();
        }
    }

    auto empty() -> bool { return m_map.empty(); }

    auto first_window() -> ::HWND {
        if (m_keys.empty()) {
            return nullptr;
        }

        return m_map.at(m_keys.front())->m_hwnd.get();
    }

    auto last_window() -> ::HWND {
        if (m_keys.empty()) {
            return nullptr;
        }

        return m_map.at(m_keys.back())->m_hwnd.get();
    }

private:
    std::vector<uintptr_t> m_keys;
    std::unordered_map<uintptr_t, std::unique_ptr<T>> m_map;
};
}; // namespace glow::window
