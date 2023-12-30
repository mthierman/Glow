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

#include <algorithm>
#include <bit>
#include <memory>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.ViewManagement.h>

#include <wil/com.h>
#include <wil/resource.h>
#include <wrl.h>

namespace glow::gui
{

struct Position
{
    int x{};
    int y{};
    int width{};
    int height{};
};

struct GdiPlus
{
    GdiPlus() { Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr); }
    ~GdiPlus() { Gdiplus::GdiplusShutdown(gdiplusToken); }

    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken{};
};

struct CoInitialize
{
    CoInitialize() : m_result{CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED)} {}
    ~CoInitialize()
    {
        if (SUCCEEDED(m_result)) CoUninitialize();
    }

    operator HRESULT() const { return m_result; }
    HRESULT m_result{};
};

template <typename T> T* InstanceFromWndProc(HWND hWnd, UINT uMsg, LPARAM lParam)
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

template <typename T> T* InstanceFromEnumChildProc(HWND hWnd, LPARAM lParam)
{
    T* self{std::bit_cast<T*>(GetWindowLongPtrA(hWnd, 0))};

    return self;
}

auto message_loop() -> int;

auto rect_to_position(RECT rect) -> Position;
auto client_rect(HWND hwnd) -> RECT;
auto window_rect(HWND hwnd) -> RECT;

auto is_dark() -> bool;

auto use_immersive_dark_mode(HWND hwnd) -> void;
auto set_system_backdrop(HWND hwnd, DWM_SYSTEMBACKDROP_TYPE backdrop) -> void;
auto cloak(HWND hwnd, bool enable) -> void;

auto clamp_color(int value) -> int;
auto make_colorref(int r, int g, int b) -> COLORREF;
auto format_color(winrt::Windows::UI::ViewManagement::UIColorType colorType) -> std::string;

auto enable_caption_color(HWND hwnd, bool enable) -> void;
auto enable_border_color(HWND hwnd, bool enable) -> void;

auto set_caption_color(HWND hwnd, COLORREF color) -> void;
auto set_border_color(HWND hwnd, COLORREF color) -> void;

auto reset_text_color(HWND hwnd) -> void;
auto set_text_color(HWND hwnd, COLORREF color) -> void;

auto set_preferred_app_mode() -> void;
auto allow_dark_mode(HWND hwnd, bool enable) -> void;

auto window_maximize(HWND hwnd) -> bool;
auto window_fullscreen(HWND hwnd) -> bool;
auto window_topmost(HWND hwnd) -> bool;

auto show_normal(HWND hwnd) -> void;
auto show(HWND hwnd) -> void;
auto hide(HWND hwnd) -> void;

auto set_title(HWND hwnd, std::string title) -> void;
auto set_icon(HWND hwnd, HICON hIcon) -> void;
auto set_border(HWND hwnd, bool enabled) -> void;
auto set_child(HWND hwnd) -> void;
auto set_popup(HWND hwnd) -> void;
auto set_overlapped(HWND hwnd) -> void;

} // namespace glow::gui
