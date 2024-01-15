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

#include <string>

#include <wil/com.h>
#include <wil/resource.h>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.ViewManagement.h>

#include <WebView2.h>
#include <WebView2EnvironmentOptions.h>

#include <nlohmann/json.hpp>

#include "console.hxx"
#include "text.hxx"

namespace glow
{
namespace gui
{
struct GdiPlus
{
    GdiPlus();
    ~GdiPlus();

    Gdiplus::GdiplusStartupInput m_gdiplusStartupInput;
    ULONG_PTR m_gdiplusToken;
    Gdiplus::Status m_gdiplusStatus;
};

struct CoInitialize
{
    CoInitialize();
    ~CoInitialize();

    operator HRESULT() const;
    HRESULT m_result;
};

struct SystemColors
{
    SystemColors();

    std::string accent;
    std::string accentDark1;
    std::string accentDark2;
    std::string accentDark3;
    std::string accentLight1;
    std::string accentLight2;
    std::string accentLight3;
    std::string background;
    std::string foreground;
};
void to_json(nlohmann::json& j, const SystemColors& systemColors);
void from_json(const nlohmann::json& j, SystemColors& systemColors);

struct Position
{
    Position();

    int x;
    int y;
    int width;
    int height;
    bool maximized;
    bool fullscreen;
    bool topmost;
    int dpi;
    float scale;
};
void to_json(nlohmann::json& j, const Position& Position);
void from_json(const nlohmann::json& j, Position& Position);

struct Notification
{
    Notification();

    NMHDR nmhdr;
    std::string message;
};
void to_json(nlohmann::json& j, const Notification& notification);
void from_json(const nlohmann::json& j, Notification& notification);

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

template <typename T> T* instance_from_wnd_proc(HWND hwnd, UINT uMsg, LPARAM lParam)
{
    T* self{nullptr};

    if (uMsg == WM_NCCREATE)
    {
        auto lpCreateStruct{reinterpret_cast<LPCREATESTRUCT>(lParam)};
        self = static_cast<T*>(lpCreateStruct->lpCreateParams);
        self->m_hwnd.reset(hwnd);
        SetWindowLongPtrA(hwnd, 0, reinterpret_cast<intptr_t>(self));
    }

    else self = reinterpret_cast<T*>(GetWindowLongPtrA(hwnd, 0));

    return self;
}

template <typename T> T* instance_from_window_long_ptr(HWND hwnd)
{
    T* self{reinterpret_cast<T*>(GetWindowLongPtrA(hwnd, 0))};

    return self;
}
} // namespace gui
} // namespace glow
