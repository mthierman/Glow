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

enum struct BackgroundType { BG_TRANSPARENT, BG_SYSTEM, BG_BLACK, BG_WHITE, BG_CUSTOM };

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

    auto set_overlapped_window() -> void;
    auto is_overlapped_window() -> bool;
    auto set_popup_window() -> void;
    auto is_popup_window() -> bool;

    auto set_popup() -> void;
    auto is_popup() -> bool;
    auto set_child() -> void;
    auto is_child() -> bool;

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

    auto set_caption_color(uint8_t r, uint8_t g, uint8_t b) -> void;
    auto set_caption_color(const winrt::Color& color) -> void;

    auto set_border_color(uint8_t r, uint8_t g, uint8_t b) -> void;
    auto set_border_color(const winrt::Color& color) -> void;

    auto set_text_color(uint8_t r, uint8_t g, uint8_t b) -> void;
    auto set_text_color(const winrt::Color& color) -> void;

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
    auto set_background_color(const winrt::Color& color) -> void;
    auto client_rect() -> ::RECT;
    auto window_rect() -> ::RECT;
    auto invalidate_rect() -> void;
    auto device_context() -> ::HDC;
    auto notify_app(glow::message::Code code,
                    std::string_view message = "",
                    ::HWND receiverHwnd
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
        wil::unique_hbrush black { glow::system::system_brush(BLACK_BRUSH) };
        wil::unique_hbrush white { glow::system::system_brush(WHITE_BRUSH) };
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

    uintptr_t id { glow::math::make_random<uintptr_t>() };
    glow::message::Manager messages;

    wil::unique_hwnd hwnd;
};

template <typename T> struct Manager {
    auto add(std::unique_ptr<T> window) -> void {
        keys.push_back(window->id);
        map.try_emplace(window->id, std::move(window));
    }

    auto remove(uintptr_t id) -> void {
        auto pos { std::find(keys.begin(), keys.end(), id) };

        if (pos != keys.end()) {
            keys.erase(pos);
        }

        map.erase(id);

        if (map.empty()) {
            glow::system::quit();
        }
    }

    auto empty() -> bool { return map.empty(); }

    auto first() -> T* {
        if (keys.empty()) {
            return nullptr;
        }

        return map.at(keys.front()).get();
    }

    auto last() -> T* {
        if (keys.empty()) {
            return nullptr;
        }

        return map.at(keys.back()).get();
    }

private:
    std::vector<uintptr_t> keys;
    std::unordered_map<uintptr_t, std::unique_ptr<T>> map;
};

auto rect_to_position(const ::RECT& rect) -> Position;
auto position_to_rect(const Position& position) -> ::RECT;
} // namespace glow::window
