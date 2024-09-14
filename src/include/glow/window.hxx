// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <Windows.h>

#include <dwmapi.h>
#include <wrl.h>

#include <algorithm>
#include <filesystem>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <wil/com.h>
#include <wil/resource.h>
#include <wil/wrl.h>

#include <WebView2.h>
#include <WebView2EnvironmentOptions.h>

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

enum struct Background { Transparent, System, Black, White, Custom };

struct Window {
    Window();

protected:
    static auto CALLBACK procedure(::HWND hwnd,
                                   ::UINT msg,
                                   ::WPARAM wparam,
                                   ::LPARAM lparam) -> ::LRESULT;

public:
    auto register_class(::WNDCLASSEXW& windowClass) -> void;
    auto refresh_dpi() -> void;
    auto erase_background(::HDC hdc) -> int;
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
    auto set_position(::RECT rect) -> void;
    auto set_window_placement() -> void;
    auto get_window_placement() -> void;
    auto get_monitor_info() -> void;
    auto set_style(::LONG_PTR style) -> void;
    auto get_style() -> ::LONG_PTR;
    auto set_ex_style(::LONG_PTR exStyle) -> void;
    auto get_ex_style() -> ::LONG_PTR;
    auto get_id() -> ::LONG_PTR;
    auto set_small_icon(::HICON hicon) -> void;
    auto set_big_icon(::HICON hicon) -> void;
    auto flash() -> void;
    auto start_timer(::UINT_PTR timerId, ::UINT intervalMs) -> bool;
    auto stop_timer(::UINT_PTR timerId) -> bool;
    auto enable_fullscreen() -> bool;
    auto disable_fullscreen() -> bool;
    auto set_background(Background background) -> void;
    auto set_background_color(uint8_t r, uint8_t g, uint8_t b) -> void;
    auto set_background_color(const winrt::Color& color) -> void;
    auto paint_background(::HDC hdc, const wil::unique_hbrush& brush) -> void;
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
        Position work;
        Position restoreCentered;
        Position restoreFullscreen;
    };
    Positions positions;

    struct States {
        Background background { Background::System };
        bool centered { false };
        bool fullscreen { false };
        bool maximized { false };
        bool minimized { false };
    };
    States states;

    ::WINDOWPLACEMENT windowPlacement {};
    ::MONITORINFO monitorInfo {};
    size_t dpi { USER_DEFAULT_SCREEN_DPI };
    double scale { 1.0 };

    struct Brushes {
        wil::unique_hbrush transparent { glow::system::system_brush() };
        wil::unique_hbrush black { glow::system::system_brush(BLACK_BRUSH) };
        wil::unique_hbrush white { glow::system::system_brush(WHITE_BRUSH) };
        wil::unique_hbrush system { glow::color::create_brush(
            glow::color::system(winrt::UIColorType::Background)) };
        wil::unique_hbrush custom;
    };
    Brushes brushes;

    struct Icons {
        wil::unique_hicon app { glow::system::resource_icon() ? glow::system::resource_icon()
                                                              : glow::system::system_icon() };
    };
    Icons icons;

    uintptr_t id { glow::math::make_random<uintptr_t>() };

    glow::message::Manager messages;
    glow::message::Manager baseMessages;
    glow::message::Manager derivedMessages;
    glow::message::Manager defaultMessages;

    wil::unique_hwnd hwnd;
};

struct Overlapped : Window {
    auto create(bool show = false) -> void;

    ::WNDCLASSEXW windowClass { .cbSize { sizeof(::WNDCLASSEXW) },
                                .style { 0 },
                                .lpfnWndProc { procedure },
                                .cbClsExtra { 0 },
                                .cbWndExtra { sizeof(Window) },
                                .hInstance { glow::system::instance() },
                                .hIcon { icons.app.get() },
                                .hCursor { glow::system::system_cursor() },
                                .hbrBackground { nullptr },
                                .lpszMenuName { nullptr },
                                .lpszClassName { L"Overlapped" },
                                .hIconSm { icons.app.get() } };
};

struct Child : Window {
    auto create(::HWND parent, bool show = false) -> void;

    ::WNDCLASSEXW windowClass { .cbSize { sizeof(::WNDCLASSEXW) },
                                .style { 0 },
                                .lpfnWndProc { procedure },
                                .cbClsExtra { 0 },
                                .cbWndExtra { sizeof(Window) },
                                .hInstance { glow::system::instance() },
                                .hIcon { icons.app.get() },
                                .hCursor { glow::system::system_cursor() },
                                .hbrBackground { nullptr },
                                .lpszMenuName { nullptr },
                                .lpszClassName { L"Child" },
                                .hIconSm { icons.app.get() } };
};

struct WebView : Window {
    using Callback = std::function<void()>;

    auto create(Callback callback = 0, bool show = false) -> void;
    auto create(::HWND parent, Callback callback = 0, bool show = false) -> void;
    auto create_webview(Callback callback = 0) -> void;

    auto put_bounds(const Position& position) -> void;
    auto put_bounds(const ::RECT& rect) -> void;
    auto put_bounds(const ::SIZE& size) -> void;
    auto put_bounds(const ::WINDOWPOS& windowPos) -> void;

    auto show_controller() -> void;
    auto hide_controller() -> void;
    auto navigate(const std::string& url) -> void;
    auto navigate(const std::wstring& url) -> void;
    auto get_document_title() -> std::string;

    template <typename T> auto event_handler(T eventHandler) {
        return wil::MakeAgileCallback<ICoreWebView2DOMContentLoadedEventHandler>(eventHandler);
    }

    auto event_token(std::string key) -> ::EventRegistrationToken*;

    ::WNDCLASSEXW windowClass { .cbSize { sizeof(::WNDCLASSEXW) },
                                .style { 0 },
                                .lpfnWndProc { procedure },
                                .cbClsExtra { 0 },
                                .cbWndExtra { sizeof(Window) },
                                .hInstance { glow::system::instance() },
                                .hIcon { icons.app.get() },
                                .hCursor { glow::system::system_cursor() },
                                .hbrBackground { nullptr },
                                .lpszMenuName { nullptr },
                                .lpszClassName { L"WebView" },
                                .hIconSm { icons.app.get() } };

    struct Config {
        struct EnvironmentOptions {
            std::string AdditionalBrowserArguments;
            bool AllowSingleSignOnUsingOSPrimaryAccount { false };
            std::string Language;
            std::string TargetCompatibleBrowserVersion;
            bool ExclusiveUserDataFolderAccess { false };
            bool IsCustomCrashReportingEnabled { false };
            bool EnableTrackingPrevention { true };
            bool AreBrowserExtensionsEnabled { false };
            COREWEBVIEW2_CHANNEL_SEARCH_KIND ChannelSearchKind {
                COREWEBVIEW2_CHANNEL_SEARCH_KIND::COREWEBVIEW2_CHANNEL_SEARCH_KIND_MOST_STABLE
            };
            COREWEBVIEW2_SCROLLBAR_STYLE ScrollBarStyle {
                COREWEBVIEW2_SCROLLBAR_STYLE::COREWEBVIEW2_SCROLLBAR_STYLE_DEFAULT
            };
        };
        EnvironmentOptions environmentOptions;

        struct Settings {
            bool AreBrowserAcceleratorKeysEnabled { true };
            bool AreDefaultContextMenusEnabled { true };
            bool AreDefaultScriptDialogsEnabled { true };
            bool AreDevToolsEnabled { true };
            bool AreHostObjectsAllowed { true };
            COREWEBVIEW2_PDF_TOOLBAR_ITEMS HiddenPdfToolbarItems {
                COREWEBVIEW2_PDF_TOOLBAR_ITEMS::COREWEBVIEW2_PDF_TOOLBAR_ITEMS_NONE
            };
            bool IsBuiltInErrorPageEnabled { true };
            bool IsGeneralAutofillEnabled { true };
            bool IsNonClientRegionSupportEnabled { true };
            bool IsPasswordAutosaveEnabled { true };
            bool IsPinchZoomEnabled { true };
            bool IsReputationCheckingRequired { true };
            bool IsScriptEnabled { true };
            bool IsStatusBarEnabled { true };
            bool IsSwipeNavigationEnabled { true };
            bool IsWebMessageEnabled { true };
            bool IsZoomControlEnabled { true };
        };
        Settings settings;

        std::filesystem::path browserExecutableFolder;
        std::filesystem::path userDataFolder;
        COREWEBVIEW2_COLOR backgroundColor { 0, 0, 0, 0 };
    };

    Config config;
    wil::com_ptr<ICoreWebView2Environment13> environment;
    wil::com_ptr<ICoreWebView2Controller4> controller;
    wil::com_ptr<ICoreWebView2_22> core;
    wil::com_ptr<ICoreWebView2Settings9> settings;
    std::unordered_map<std::string, ::EventRegistrationToken> eventTokens;
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

auto to_position(const ::RECT& rect) -> Position;
auto to_position(const ::WINDOWPOS& windowPos) -> Position;
auto to_position(const ::SIZE& size) -> Position;

auto to_rect(const Position& position) -> ::RECT;
auto to_rect(const ::WINDOWPOS& windowPos) -> ::RECT;
auto to_rect(const ::SIZE& size) -> ::RECT;
} // namespace glow::window
