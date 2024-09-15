// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <Windows.h>

#include <combaseapi.h>

#include <format>
#include <string>

#include <winrt/Windows.Foundation.h>

#include <winrt/Windows.UI.ViewManagement.h>

#include <WebView2.h>

#include <glow/system.hxx>

namespace winrt {
using namespace winrt::Windows::UI;
using namespace winrt::Windows::UI::ViewManagement;
}; // namespace winrt

namespace glow::color {
struct Color {
    Color() = default;
    Color(uint8_t r, uint8_t g, uint8_t b);
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    Color(winrt::UIColorType colorType);
    Color(winrt::UIElementType elementType);
    Color(const winrt::Color& color);
    Color(const ::COLORREF& colorRef);
    Color(const COREWEBVIEW2_COLOR& coreWebView2Color);

    uint8_t r { 0 };
    uint8_t g { 0 };
    uint8_t b { 0 };
    uint8_t a { 0 };

    auto brush() const -> ::HBRUSH;
    auto hex() const -> std::string;
    auto colorref() const -> ::COLORREF;
    auto winrt_color() const -> winrt::Color;
    auto webview2_color() const -> COREWEBVIEW2_COLOR;
    auto is_dark() const -> bool;

protected:
    winrt::UISettings uiSettings { glow::system::ui_settings() };
};
}; // namespace glow::color

namespace std {
template <> struct formatter<glow::color::Color> : formatter<string_view> {
    auto format(const glow::color::Color& color, format_context& context) const noexcept {
        return formatter<string_view>::format(color.hex(), context);
    }
};

template <> struct formatter<glow::color::Color, wchar_t> : formatter<wstring_view, wchar_t> {
    auto format(const glow::color::Color& color, wformat_context& context) const noexcept {
        return formatter<wstring_view, wchar_t>::format(glow::text::to_wstring(color.hex()), context);
    }
};
} // namespace std
