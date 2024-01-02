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
#include <ShlObj.h>
#include <wrl.h>

#include <wil/com.h>
#include <wil/result.h>
#include <wil/resource.h>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.ViewManagement.h>

#include <algorithm>
#include <bit>
#include <memory>

#include <WebView2.h>
#include <WebView2EnvironmentOptions.h>

#include <nlohmann/json.hpp>

#include <glow/console.hxx>
#include <glow/text.hxx>

namespace glow::window
{
using json = nlohmann::json;

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

struct Window
{
    Window(std::string className = "Window");
    virtual ~Window();

    void operator()(bool show = false)
    {
        create();
        if (show) show_normal();
    }

    virtual auto create() -> void;

    static auto CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    virtual auto handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;

    virtual auto on_close(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int;
    virtual auto on_destroy(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int;

    auto dpi() -> int;
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

    std::string m_className;
    wil::unique_hwnd m_hwnd{};
    std::string m_url{"https://www.google.com/"};

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
    MainWindow(std::string className = "MainWindow");

    auto on_destroy(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int;
};

struct WebView : public Window
{
    WebView(int64_t id, HWND parent, std::string url = "https://www.google.com/",
            std::string className = "WebView");

    virtual auto create() -> void override;

    auto create_environment() -> void;
    auto create_controller(ICoreWebView2Environment* environment) -> void;

    auto handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;

    auto on_size() -> int;

    virtual auto initialized() -> void{};

    auto navigate(std::string url) -> void;
    auto post_json(const json jsonMessage) -> void;

    auto source_changed() -> void;
    virtual auto source_changed_handler() -> void{};

    auto navigation_completed() -> void;
    virtual auto navigation_completed_handler() -> void{};

    auto web_message_received() -> void;
    virtual auto web_message_received_handler() -> void{};

    auto accelerator_key_pressed() -> void;
    virtual auto accelerator_key_pressed_handler(ICoreWebView2AcceleratorKeyPressedEventArgs* args)
        -> void{};

    auto favicon_changed() -> void;
    virtual auto favicon_changed_handler() -> void{};

    auto document_title_changed() -> void;
    virtual auto document_title_changed_handler() -> void{};

    int64_t m_id{};
    HWND m_parent{};

    bool m_initialized{false};

    wil::com_ptr<ICoreWebView2EnvironmentOptions6> m_evironmentOptions6{nullptr};
    wil::com_ptr<ICoreWebView2Controller> m_controller{nullptr};
    wil::com_ptr<ICoreWebView2Controller4> m_controller4{nullptr};
    wil::com_ptr<ICoreWebView2> m_core{nullptr};
    wil::com_ptr<ICoreWebView2_20> m_core20{nullptr};
    wil::com_ptr<ICoreWebView2Settings> m_settings{nullptr};
    wil::com_ptr<ICoreWebView2Settings8> m_settings8{nullptr};
};

auto message_loop() -> int;

auto rect_to_position(const RECT& rect) -> Position;
auto position_to_rect(const Position& position) -> RECT;

auto clamp_color(int value) -> int;
auto make_colorref(int r, int g, int b) -> COLORREF;
auto format_color(winrt::Windows::UI::ViewManagement::UIColorType colorType) -> std::string;

auto check_theme() -> bool;

auto set_preferred_app_mode() -> void;
auto allow_dark_mode(HWND hwnd, bool enable) -> void;

auto icon_application() -> HICON;
auto icon_error() -> HICON;
auto icon_question() -> HICON;
auto icon_warning() -> HICON;
auto icon_information() -> HICON;
auto icon_winlogo() -> HICON;
auto icon_shield() -> HICON;

struct GdiPlus
{
    GdiPlus();
    ~GdiPlus();

    Gdiplus::GdiplusStartupInput m_gdiplusStartupInput;
    ULONG_PTR m_gdiplusToken{};
    Gdiplus::Status m_gdiplusStatus;
};

struct CoInitialize
{
    CoInitialize();
    ~CoInitialize();

    operator HRESULT() const;
    HRESULT m_result{};
};

} // namespace glow::window
