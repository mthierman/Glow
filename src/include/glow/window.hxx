// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <Windows.h>

#include <dwmapi.h>

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <wil/resource.h>

#include <glow/color.hxx>
#include <glow/message.hxx>

namespace glow::window {
struct Messages {
    using Callback = std::function<::LRESULT(glow::message::Message)>;

    auto on(::UINT msg, Callback callback) -> bool;
    auto contains(::UINT msg) -> bool;
    auto invoke(glow::message::Message message) -> ::LRESULT;

    template <typename W, typename L>::LRESULT send(::HWND hwnd, ::UINT msg, W wparam, L lparam) {
        return ::SendMessageW(hwnd, msg, (::WPARAM)wparam, (::LPARAM)lparam);
    }

    template <typename W, typename L>::LRESULT post(::HWND hwnd, ::UINT msg, W wparam, L lparam) {
        return ::PostMessageW(hwnd, msg, (::WPARAM)wparam, (::LPARAM)lparam);
    }

private:
    std::unordered_map<::UINT, Callback> map;
};

struct Position {
    int x { 0 };
    int y { 0 };
    int width { 0 };
    int height { 0 };
};

struct Window {
    auto create(std::string_view title = "Window") -> void;

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
    auto center() -> void;
    auto top() -> void;
    auto bottom() -> void;
    auto topmost() -> void;
    auto no_topmost() -> void;
    auto cloak() -> void;
    auto uncloak() -> void;
    auto enable_dark_mode() -> void;
    auto disable_dark_mode() -> void;
    auto set_backdrop(::DWM_SYSTEMBACKDROP_TYPE backdrop) -> void;
    auto set_round_corners(::DWM_WINDOW_CORNER_PREFERENCE corner) -> void;
    auto set_caption_color(::COLORREF color) -> void;
    auto set_border_color(::COLORREF color) -> void;
    auto set_text_color(::COLORREF color) -> void;
    auto focus() -> void;
    auto is_focused() -> bool;
    auto foreground() -> void;
    auto is_foreground() -> bool;
    auto active() -> void;
    auto is_active() -> bool;
    auto set_parent(::HWND parent) -> void;
    auto get_parent() -> ::HWND;
    auto bring_to_top() -> void;
    auto is_topmost() -> bool;
    auto is_visible() -> bool;
    auto is_cloaked() -> bool;
    auto is_maximized() -> bool;
    auto set_title(const std::string& title) -> void;
    auto set_position(Position position) -> void;
    auto set_placement() -> void;
    auto get_placement() -> void;
    auto set_style(::LONG_PTR style) -> void;
    auto get_style() -> ::LONG_PTR;
    auto set_ex_style(::LONG_PTR exStyle) -> void;
    auto get_ex_style() -> ::LONG_PTR;
    auto get_id() -> ::LONG_PTR;
    auto set_small_icon(::HICON hicon) -> void;
    auto set_big_icon(::HICON hicon) -> void;
    auto flash() -> void;
    auto timer_start(::UINT_PTR timerId, ::UINT intervalMs) -> bool;
    auto timer_stop(::UINT_PTR timerId) -> bool;
    auto close() -> void;

    auto enable_fullscreen() -> bool;
    auto disable_fullscreen() -> bool;

    auto enable_custom_background(uint8_t r, uint8_t g, uint8_t b) -> void;
    auto disable_custom_background() -> void;

    auto enable_system_background() -> void;
    auto disable_system_background() -> void;

    auto client_rect() -> ::RECT;
    auto window_rect() -> ::RECT;
    auto invalidate_rect() -> void;

    auto device_context() -> ::HDC;

    struct Positions {
        Position window;
        Position client;
        Position monitor;
        Position restore;
        ::WINDOWPLACEMENT placement;
        uint64_t dpi { USER_DEFAULT_SCREEN_DPI };
        double scale { 1.0 };
        bool fullscreen { false };
    };
    Positions positions;

    struct Backgrounds {
        wil::unique_hbrush system;
        wil::unique_hbrush custom;
    };
    Backgrounds backgrounds;

    Messages messages;
    wil::unique_hwnd hwnd;
};

template <typename T> struct Manager {
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

auto default_procedure(glow::message::Message message) -> ::LRESULT;
} // namespace glow::window

namespace glow::window {
// auto find_message_only(const std::string& name = "MessageWindow") -> ::HWND;

// auto set_overlapped(::HWND hwnd) -> void;
// auto check_overlapped(::HWND hwnd) -> bool;

// auto set_child(::HWND hwnd) -> void;
// auto check_child(::HWND hwnd) -> bool;

// auto set_popup(::HWND hwnd) -> void;
// auto check_popup(::HWND hwnd) -> bool;

// auto add_border(::HWND hwnd) -> void;
// auto remove_border(::HWND hwnd) -> void;

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
}; // namespace glow::window
