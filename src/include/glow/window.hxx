// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <Windows.h>

#include <combaseapi.h>

#include <dwmapi.h>
#include <wrl.h>

#include <algorithm>
#include <any>
#include <filesystem>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <wil/com.h>
#include <wil/resource.h>
#include <wil/windowing.h>
#include <wil/wrl.h>

#include <WebView2.h>
#include <WebView2EnvironmentOptions.h>

#include <glow/color.hxx>
#include <glow/math.hxx>
#include <glow/message.hxx>
#include <glow/system.hxx>

namespace glow::window {
struct Position final {
    Position() = default;
    Position(int x, int y, int width, int height);
    Position(const ::RECT& rect);
    Position(const ::SIZE& size);
    Position(const ::WINDOWPOS& windowPos);

    auto rect() const -> ::RECT;

    int x { 0 };
    int y { 0 };
    int width { 0 };
    int height { 0 };
};

struct Size final {
    int x { 0 };
    int y { 0 };
};

struct State final {
    bool centered { false };
    bool fullscreen { false };
    bool maximized { false };
    bool minimized { false };
    bool darkMode { glow::system::is_dark() };
};

struct Background final {
    enum struct Style { Transparent, System, Custom };

    struct Brush {
        wil::unique_hbrush transparent { glow::system::system_brush() };
        wil::unique_hbrush dark { glow::system::system_brush(BLACK_BRUSH) };
        wil::unique_hbrush light { glow::system::system_brush(WHITE_BRUSH) };
        wil::unique_hbrush custom { glow::system::system_brush() };
    };

    struct Color {
        glow::color::Color transparent;
        glow::color::Color dark { 0, 0, 0, 255 };
        glow::color::Color light { 255, 255, 255, 255 };
        glow::color::Color custom;
    };

    Style style { Style::System };
    Brush brush;
    Color color;
};

struct Window {
    struct Positions {
        Position window;
        Position client;
        Position monitor;
        Position work;
        Position restoreCentered;
        Position restoreFullscreen;
    };

    struct Icons {
        wil::unique_hicon app { glow::system::resource_icon() ? glow::system::resource_icon()
                                                              : glow::system::system_icon() };
    };

    Window();
    virtual ~Window() = default;

protected:
    static auto CALLBACK procedure(::HWND hwnd, ::UINT msg, ::WPARAM wparam, ::LPARAM lparam)
        -> ::LRESULT;

public:
    auto theme_refresh() -> void;
    auto background_refresh() -> void;
    auto caption_refresh() -> void;
    auto frame_refresh() -> void;
    auto window_refresh() -> void;
    auto dpi_refresh() -> void;
    auto paint_background(::HDC hdc, ::HBRUSH brush) -> void;
    auto caption_color(const glow::color::Color& color) -> void;
    auto text_color(const glow::color::Color& color) -> void;
    auto border_color(const glow::color::Color& color) -> void;

    auto background_style(Background::Style style) -> void;
    auto background_dark(const glow::color::Color& color) -> void;
    auto background_light(const glow::color::Color& color) -> void;
    auto background_custom(const glow::color::Color& color) -> void;
    auto activate() -> void;
    auto close() -> void;
    auto show() -> void;
    auto hide() -> void;
    auto maximize() -> void;
    auto minimize() -> void;
    auto restore() -> void;
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
    auto set_title(std::u8string_view title) -> void;
    auto set_position(const Position& position) -> void;
    auto min_size(int x, int y) -> void;
    auto max_size(int x, int y) -> void;
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
    auto start_timer(uintptr_t timerId, unsigned int intervalMs) -> bool;
    auto stop_timer(uintptr_t timerId) -> bool;
    auto enable_fullscreen() -> bool;
    auto disable_fullscreen() -> bool;
    auto client_position() -> Position;
    auto window_position() -> Position;
    auto device_context() -> ::HDC;
    auto notify_app(glow::message::Code code,
                    std::u8string_view message = u8"",
                    ::HWND receiverHwnd
                    = ::FindWindowExW(HWND_MESSAGE, nullptr, L"MessageWindow", nullptr)) -> void;

    Positions positions;
    State state;
    size_t dpi { USER_DEFAULT_SCREEN_DPI };
    double scale { 1.0 };
    Size minTrackSize {};
    Size maxTrackSize {};

protected:
    ::WINDOWPLACEMENT windowPlacement {};
    ::MONITORINFO monitorInfo {};
    Background background;
    Icons icons;
    glow::message::Manager baseMessages;
    glow::message::Manager derivedMessages;
    glow::message::Manager defaultMessages;

public:
    glow::message::Manager messages;
    uintptr_t id { glow::math::make_random<uintptr_t>() };
    wil::unique_hwnd hwnd;
};

struct Overlapped : Window {
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
                                .lpszClassName { L"OverlappedWindow" },
                                .hIconSm { icons.app.get() } };

    Overlapped() = default;
    virtual ~Overlapped() = default;

    auto create(bool show = true) -> void;
};

struct Child : Window {
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
                                .lpszClassName { L"ChildWindow" },
                                .hIconSm { icons.app.get() } };

    Child() = default;
    virtual ~Child() = default;

    auto create(::HWND parent, bool show = true) -> void;
};

struct WebView : Window {
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
                                .lpszClassName { L"WebViewWindow" },
                                .hIconSm { icons.app.get() } };

    struct Config {
        struct EnvironmentOptions {
            std::u8string AdditionalBrowserArguments;
            bool AllowSingleSignOnUsingOSPrimaryAccount { false };
            std::u8string Language;
            std::u8string TargetCompatibleBrowserVersion;
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

        EnvironmentOptions environmentOptions;
        Settings settings;
        std::u8string homePage { u8"about:blank" };
        std::filesystem::path browserExecutableFolder;
        std::filesystem::path userDataFolder;
    };

    struct Event {
        struct Token {
            auto operator()(std::u8string_view key) -> ::EventRegistrationToken*;

        private:
            std::unordered_map<std::u8string, ::EventRegistrationToken> eventRegistrationTokens;
        };

        template <typename T> auto make(auto&&... eventHandler) {
            auto key { eventHandlers.size() };
            eventHandlers.try_emplace(
                key,
                wil::MakeAgileCallback<T>(std::forward<decltype(eventHandler)>(eventHandler)...));

            return std::any_cast<Microsoft::WRL::ComPtr<T>>(eventHandlers.at(key)).Get();
        }

        Token token;

    private:
        std::unordered_map<size_t, std::any> eventHandlers;
    };

    WebView() = default;
    virtual ~WebView() = default;

    using Callback = std::function<void()>;

    auto create(Callback callback = 0, bool show = true) -> void;
    auto create(::HWND parent, Callback callback = 0, bool show = true) -> void;

protected:
    auto create_webview(Callback&& callback = 0) -> void;

public:
    auto put_bounds(const Position& position) -> void;

    auto show_controller() -> void;
    auto hide_controller() -> void;

    auto get_document_title() -> std::string;

    auto navigate(std::u8string_view url) -> void;
    auto navigate_to_string(std::u8string_view url) -> void;
    auto virtual_host_name_mapping(
        const std::string& hostName,
        const std::filesystem::path& folder,
        COREWEBVIEW2_HOST_RESOURCE_ACCESS_KIND accessKind
        = COREWEBVIEW2_HOST_RESOURCE_ACCESS_KIND::COREWEBVIEW2_HOST_RESOURCE_ACCESS_KIND_DENY)
        -> void;
    auto virtual_host_name_mapping(
        const std::wstring& hostName,
        const std::filesystem::path& folder,
        COREWEBVIEW2_HOST_RESOURCE_ACCESS_KIND accessKind
        = COREWEBVIEW2_HOST_RESOURCE_ACCESS_KIND::COREWEBVIEW2_HOST_RESOURCE_ACCESS_KIND_DENY)
        -> void;
    auto clear_virtual_host_name_mapping(const std::string& hostName) -> void;
    auto clear_virtual_host_name_mapping(const std::wstring& hostName) -> void;

    auto suspend(Callback callback = 0) -> void;
    auto is_suspended() -> bool;
    auto resume() -> void;

    Event event;
    Config config;
    wil::com_ptr<ICoreWebView2Environment13> environment;
    wil::com_ptr<ICoreWebView2Controller4> controller;
    wil::com_ptr<ICoreWebView2_22> core;
    wil::com_ptr<ICoreWebView2Settings9> settings;
};

struct Message {
    ::WNDCLASSEXW windowClass { .cbSize { sizeof(::WNDCLASSEXW) },
                                .style { 0 },
                                .lpfnWndProc { procedure },
                                .cbClsExtra { 0 },
                                .cbWndExtra { sizeof(Message) },
                                .hInstance { glow::system::instance() },
                                .hIcon { nullptr },
                                .hCursor { nullptr },
                                .hbrBackground { nullptr },
                                .lpszMenuName { nullptr },
                                .lpszClassName { L"MessageWindow" },
                                .hIconSm { nullptr } };

    Message() = default;
    virtual ~Message() = default;

protected:
    static auto CALLBACK procedure(::HWND hwnd, ::UINT msg, ::WPARAM wparam, ::LPARAM lparam)
        -> ::LRESULT;

public:
    auto create() -> void;

    auto operator()() -> int;

    auto notify_app(glow::message::Code code,
                    std::u8string_view message = u8"",
                    ::HWND receiverHwnd
                    = ::FindWindowExW(HWND_MESSAGE, nullptr, L"MessageWindow", nullptr)) -> void;

    uintptr_t id { glow::math::make_random<uintptr_t>() };
    glow::message::Manager messages;

    wil::unique_hwnd hwnd;
};

template <typename T> struct Manager final {
    auto add(auto&& factory) -> void {
        auto window { std::forward<decltype(factory)>(factory)() };
        keys.push_back(window->id);
        map.try_emplace(window->id, std::move(window));
    }

    auto remove(uintptr_t id) -> void {
        auto pos { std::find(keys.begin(), keys.end(), id) };

        if (pos != keys.end()) {
            keys.erase(pos);
        }

        map.at(id)->close();

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
} // namespace glow::window
