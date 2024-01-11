// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#include <Windows.h>
#include <Unknwn.h>
#include <dwmapi.h>
#include <gdiplus.h>
#include <objbase.h>
#include <shellapi.h>
#include <comdef.h>
#include <ShlObj.h>
#include <wrl.h>

#include <wil/com.h>
#include <wil/result.h>
#include <wil/resource.h>
#include <wil/win32_helpers.h>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.ViewManagement.h>

#include <WebView2.h>
#include <WebView2EnvironmentOptions.h>

#include <algorithm>
#include <bit>
#include <cstdint>
#include <filesystem>
#include <format>
#include <iostream>
#include <limits>
#include <memory>
#include <optional>
#include <print>
#include <random>
#include <source_location>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>
#include <sqlite3.h>

namespace glow
{
namespace text
{
auto narrow(std::wstring utf16) -> std::string;
auto widen(std::string utf8) -> std::wstring;
auto randomize(std::string string) -> std::string;
auto random_int64() -> int64_t;
auto random_int32() -> int32_t;
auto create_guid() -> GUID;
auto guid_to_string(GUID guid) -> std::string;
} // namespace text

namespace console
{
struct Console
{
    Console();
    ~Console();
    FILE* pFile;
};
auto argv() -> std::vector<std::string>;
auto hresult_to_string(HRESULT errorCode) -> std::string;
auto hresult_check(HRESULT errorCode) -> HRESULT;
auto debug_hresult(HRESULT errorCode,
                   std::source_location location = std::source_location::current()) -> void;
auto print_hresult(HRESULT errorCode,
                   std::source_location location = std::source_location::current()) -> void;
auto debug(std::string message, std::source_location location = std::source_location::current())
    -> void;
auto print(std::string message, std::source_location location = std::source_location::current())
    -> void;
auto box(std::string message, UINT type = MB_OK | MB_ICONINFORMATION) -> int;
auto shell(std::string message, UINT type = MB_OK | MB_ICONINFORMATION) -> int;
auto stock(std::string message, SHSTOCKICONID icon = SIID_INFO) -> void;
} // namespace console

namespace filesystem
{
struct Database
{
    Database();
    ~Database();
    auto open() -> void;
    auto write() -> void;

  private:
    struct sqlite3_deleter
    {
        auto operator()(sqlite3* pDb) noexcept -> void { sqlite3_close(pDb); }
    };
    using sqlite3_ptr = std::unique_ptr<sqlite3, sqlite3_deleter>;
    sqlite3_ptr p_db;
    std::filesystem::path path;
};
auto known_folder(KNOWNFOLDERID folderId = FOLDERID_LocalAppData) -> std::filesystem::path;
auto program_name() -> std::string;
auto portable() -> std::filesystem::path;
auto wportable() -> std::filesystem::path;
} // namespace filesystem

namespace gui
{
struct Position;
struct Colors;
struct Window;
struct MainWindow;
struct WebView;
struct GdiPlus;
struct CoInitialize;
auto message_loop() -> int;
auto webview_version() -> std::string;
auto get_class_info(ATOM& atom, WNDCLASSEXA& wndClass) -> bool;
auto rect_to_position(const RECT& rect) -> Position;
auto position_to_rect(const Position& position) -> RECT;
auto clamp_color(int value) -> int;
auto make_colorref(int r, int g, int b) -> COLORREF;
auto color_to_string(winrt::Windows::UI::ViewManagement::UIColorType colorType) -> std::string;
auto check_theme() -> bool;
auto set_preferred_app_mode() -> void;
auto allow_dark_mode(HWND hWnd, bool enable) -> void;
auto icon_application() -> HICON;
auto icon_error() -> HICON;
auto icon_question() -> HICON;
auto icon_warning() -> HICON;
auto icon_information() -> HICON;
auto icon_winlogo() -> HICON;
auto icon_shield() -> HICON;
template <typename T> T* instance_from_wnd_proc(HWND hWnd, UINT uMsg, LPARAM lParam)
{
    T* self{nullptr};

    if (uMsg == WM_NCCREATE)
    {
        auto lpCreateStruct{std::bit_cast<LPCREATESTRUCT>(lParam)};
        self = static_cast<T*>(lpCreateStruct->lpCreateParams);
        self->m_hwnd.reset(hWnd);
        SetWindowLongPtrA(hWnd, 0, std::bit_cast<LONG_PTR>(self));
    }

    else self = std::bit_cast<T*>(GetWindowLongPtrA(hWnd, 0));

    return self;
}
template <typename T> T* instance(HWND hWnd)
{
    T* self{std::bit_cast<T*>(GetWindowLongPtrA(hWnd, 0))};

    return self;
}
struct Position
{
    int x{};
    int y{};
    int width{};
    int height{};
};
void to_json(nlohmann::json& j, const Position& position);
void from_json(const nlohmann::json& j, Position& position);
struct Colors
{
    std::string accent{color_to_string(winrt::Windows::UI::ViewManagement::UIColorType::Accent)};
    std::string accentDark1{
        color_to_string(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark1)};
    std::string accentDark2{
        color_to_string(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark2)};
    std::string accentDark3{
        color_to_string(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark3)};
    std::string accentLight1{
        color_to_string(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight1)};
    std::string accentLight2{
        color_to_string(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight2)};
    std::string accentLight3{
        color_to_string(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight3)};
    std::string background{
        color_to_string(winrt::Windows::UI::ViewManagement::UIColorType::Background)};
    std::string foreground{
        color_to_string(winrt::Windows::UI::ViewManagement::UIColorType::Foreground)};
};
void to_json(nlohmann::json& j, const Colors& colors);
void from_json(const nlohmann::json& j, Colors& colors);
struct Window
{
    Window(std::string className = "Window", bool show = true);
    virtual ~Window();
    virtual auto register_window() -> void;
    virtual auto create_window() -> void;
    virtual auto operator()(bool show = true) -> void;
    static auto CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    virtual auto handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    virtual auto on_close(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int;
    auto dpi() -> int64_t;
    auto scale() -> float;
    auto show_normal() -> void;
    auto show() -> void;
    auto hide() -> void;
    auto maximize() -> bool;
    auto fullscreen() -> bool;
    auto topmost() -> bool;
    auto title(std::string title) -> void;
    auto icon(HICON hIcon) -> void;
    auto border(bool enabled) -> void;
    auto overlapped() -> void;
    auto popup() -> void;
    auto child() -> void;
    auto reparent(HWND parent) -> void;
    auto client_rect() -> RECT;
    auto window_rect() -> RECT;
    auto client_position() -> Position;
    auto window_position() -> Position;
    auto dwm_dark_mode(bool enabled) -> void;
    auto dwm_system_backdrop(DWM_SYSTEMBACKDROP_TYPE backdrop) -> void;
    auto dwm_rounded_corners(bool enabled) -> void;
    auto dwm_cloak(bool enable) -> void;
    auto dwm_caption_color(bool enable) -> void;
    auto dwm_set_caption_color(COLORREF color) -> void;
    auto dwm_border_color(bool enable) -> void;
    auto dwm_set_border_color(COLORREF color) -> void;
    auto dwm_set_text_color(COLORREF color) -> void;
    auto dwm_reset_text_color() -> void;
    std::string m_className{"Window"};
    bool m_show{true};
    WNDCLASSEXA m_wcex{sizeof(WNDCLASSEXA)};
    wil::unique_hwnd m_hwnd{nullptr};
    int64_t m_id{glow::text::random_int64()};
    int64_t m_dpi{};
    float m_scale{};
    Position m_position;
    Colors m_colors;
    wil::unique_hcursor m_hCursor{static_cast<HCURSOR>(
        LoadImageA(nullptr, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED | LR_DEFAULTSIZE))};
    wil::unique_hicon m_hIcon{static_cast<HICON>(
        LoadImageA(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE))};
    wil::unique_hicon m_appIcon{static_cast<HICON>(LoadImageA(
        GetModuleHandleA(nullptr), MAKEINTRESOURCEA(101), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE))};
    wil::unique_hbrush m_hbrBackgroundNull{static_cast<HBRUSH>(GetStockObject(NULL_BRUSH))};
    wil::unique_hbrush m_hbrBackgroundBlack{static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH))};
    wil::unique_hbrush m_hbrBackgroundWhite{static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH))};
};
struct MainWindow : public Window
{
    MainWindow(std::string className = "MainWindow", bool show = true);
    virtual auto handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        -> LRESULT override;
    virtual auto on_destroy(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int;
};
struct WebView : public Window
{
    struct WebView2
    {
        wil::com_ptr<ICoreWebView2EnvironmentOptions6> m_evironmentOptions6{nullptr};
        wil::com_ptr<ICoreWebView2Controller> m_controller{nullptr};
        wil::com_ptr<ICoreWebView2Controller4> m_controller4{nullptr};
        wil::com_ptr<ICoreWebView2> m_core{nullptr};
        wil::com_ptr<ICoreWebView2_20> m_core20{nullptr};
        wil::com_ptr<ICoreWebView2Settings> m_settings{nullptr};
        wil::com_ptr<ICoreWebView2Settings8> m_settings8{nullptr};
    };
    WebView(int64_t id, HWND parent, std::string url = "about:blank",
            std::string className = "WebView", bool show = true);
    virtual auto operator()(bool show = true) -> void override;
    virtual auto register_window() -> void override;
    virtual auto create_window() -> void override;
    auto handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    virtual auto on_size(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int;
    virtual auto create_webview() -> HRESULT;
    virtual auto configure_settings() -> HRESULT;
    virtual auto create_controller(ICoreWebView2Environment* createdEnvironment) -> HRESULT;
    virtual auto initialized() -> void {}
    auto context_menu_requested() -> HRESULT;
    virtual auto context_menu_requested_handler(ICoreWebView2* sender,
                                                ICoreWebView2ContextMenuRequestedEventArgs* args)
        -> HRESULT
    {
        return S_OK;
    }
    auto source_changed() -> HRESULT;
    virtual auto source_changed_handler(ICoreWebView2* sender,
                                        ICoreWebView2SourceChangedEventArgs* args) -> HRESULT
    {
        return S_OK;
    }
    auto navigation_starting() -> HRESULT;
    virtual auto navigation_starting_handler(ICoreWebView2* sender,
                                             ICoreWebView2NavigationStartingEventArgs* args)
        -> HRESULT
    {
        return S_OK;
    }
    auto navigation_completed() -> HRESULT;
    virtual auto navigation_completed_handler(ICoreWebView2* sender,
                                              ICoreWebView2NavigationCompletedEventArgs* args)
        -> HRESULT
    {
        return S_OK;
    }
    auto web_message_received() -> HRESULT;
    virtual auto web_message_received_handler(ICoreWebView2* sender,
                                              ICoreWebView2WebMessageReceivedEventArgs* args)
        -> HRESULT
    {
        return S_OK;
    }
    auto document_title_changed() -> HRESULT;
    virtual auto document_title_changed_handler(ICoreWebView2* sender, IUnknown* args) -> HRESULT
    {
        return S_OK;
    }
    auto favicon_changed() -> HRESULT;
    virtual auto favicon_changed_handler(ICoreWebView2* sender, IUnknown* args) -> HRESULT
    {
        return S_OK;
    }
    auto accelerator_key_pressed() -> HRESULT;
    virtual auto accelerator_key_pressed_handler(ICoreWebView2Controller* sender,
                                                 ICoreWebView2AcceleratorKeyPressedEventArgs* args)
        -> HRESULT
    {
        return S_OK;
    }
    auto zoom_factor_changed() -> HRESULT;
    virtual auto zoom_factor_changed_handler(ICoreWebView2Controller* sender, IUnknown* args)
        -> HRESULT
    {
        return S_OK;
    }
    auto navigate(std::string url) -> HRESULT;
    auto post_json(const nlohmann::json jsonMessage) -> HRESULT;
    HWND m_parent{nullptr};
    std::string m_url{"about:blank"};
    bool m_initialized{false};
    WebView2 m_webView;
};
struct GdiPlus
{
    GdiPlus();
    ~GdiPlus();

    Gdiplus::GdiplusStartupInput m_gdiplusStartupInput{};
    ULONG_PTR m_gdiplusToken{};
    Gdiplus::Status m_gdiplusStatus{};
};
struct CoInitialize
{
    CoInitialize();
    ~CoInitialize();

    operator HRESULT() const;
    HRESULT m_result{};
};
} // namespace gui

} // namespace glow
