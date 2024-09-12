// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <Windows.h>

#include <dwmapi.h>

#include <algorithm>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <wil/resource.h>

#include <glow/color.hxx>
#include <glow/math.hxx>
#include <glow/message.hxx>
#include <glow/system.hxx>

namespace glow::window {
struct Position {
    int x { 0 };
    int y { 0 };
    int width { 0 };
    int height { 0 };
};

enum struct BackgroundType { BG_TRANSPARENT, BG_SYSTEM, BG_CUSTOM };

struct Window {
    auto create() -> void;
    auto create(::HWND parent) -> void;

private:
    static auto CALLBACK procedure(::HWND hwnd,
                                   ::UINT msg,
                                   ::WPARAM wparam,
                                   ::LPARAM lparam) -> ::LRESULT;

public:
    auto refresh_dpi() -> void;

    auto activate() -> void;
    auto show() -> void;
    auto hide() -> void;
    auto maximize() -> void;
    auto minimize() -> void;
    auto restore() -> void;

    auto refresh_frame() -> void;
    auto center() -> void;
    auto top() -> void;
    auto bottom() -> void;
    auto enable_topmost() -> void;
    auto disable_topmost() -> void;
    auto enable_border() -> void;
    auto disable_border() -> void;

    auto cloak() -> void;
    auto uncloak() -> void;
    auto is_cloaked() -> bool;

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
    auto set_background_type(BackgroundType backgroundType) -> void;
    auto set_background_color(uint8_t r, uint8_t g, uint8_t b) -> void;
    auto client_rect() -> ::RECT;
    auto window_rect() -> ::RECT;
    auto invalidate_rect() -> void;
    auto device_context() -> ::HDC;
    auto notify_app(glow::message::Code code,
                    std::string_view message,
                    ::HWND receiver
                    = ::FindWindowExW(HWND_MESSAGE, nullptr, L"App", nullptr)) -> void;

    struct Positions {
        Position window;
        Position client;
        Position monitor;
        Position restoreCentered;
        Position restoreFullscreen;
    };
    Positions positions;

    struct States {
        bool centered { false };
        bool fullscreen { false };
        bool maximized { false };
        bool minimized { false };
    };
    States states;

    struct Backgrounds {
        BackgroundType type { BackgroundType::BG_SYSTEM };
        wil::unique_hbrush transparent { glow::system::system_brush() };
        wil::unique_hbrush system { glow::color::create_brush(
            glow::color::system(winrt::UIColorType::Background)) };
        wil::unique_hbrush custom;
    };
    Backgrounds backgrounds;

    struct Resources {
        wil::unique_hicon icon { glow::system::resource_icon() };
    };
    Resources resources;

    ::WINDOWPLACEMENT placement;
    size_t dpi { USER_DEFAULT_SCREEN_DPI };
    double scale { 1.0 };

    glow::message::Manager messages;
    uintptr_t id { glow::math::make_random<uintptr_t>() };

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
// auto set_overlapped(::HWND hwnd) -> void;
// auto check_overlapped(::HWND hwnd) -> bool;
// auto set_child(::HWND hwnd) -> void;
// auto check_child(::HWND hwnd) -> bool;
// auto set_popup(::HWND hwnd) -> void;
// auto check_popup(::HWND hwnd) -> bool;
}; // namespace glow::window
