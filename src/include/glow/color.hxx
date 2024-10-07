// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <Windows.h>

#include <combaseapi.h>

// #include <format>
#include <string>

#include <winrt/Windows.Foundation.h>

#include <winrt/Windows.UI.ViewManagement.h>

#include <WebView2.h>

#include <glow/system.hxx>
#include <glow/text.hxx>

namespace winrt {
using namespace winrt::Windows::UI;
using namespace winrt::Windows::UI::ViewManagement;
}; // namespace winrt

namespace glow::color {
struct Color final {
    Color() = default;
    ~Color() = default;
    Color(const Color& color) = default;
    Color(Color&& color) = default;

    explicit Color(uint8_t r, uint8_t g, uint8_t b);
    explicit Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    explicit Color(winrt::UIColorType colorType);
    explicit Color(winrt::UIElementType elementType);
    explicit Color(const winrt::Color& color);
    explicit Color(const ::COLORREF& colorRef);
    explicit Color(const COREWEBVIEW2_COLOR& coreWebView2Color);

    auto operator=(const Color& color) -> Color& = default;
    auto operator=(Color&& color) -> Color& = default;

    uint8_t r { 0 };
    uint8_t g { 0 };
    uint8_t b { 0 };
    uint8_t a { 0 };

    auto brush() const -> ::HBRUSH;
    auto hex() const -> std::u8string;
    auto colorref() const -> ::COLORREF;
    auto winrt_color() const -> winrt::Color;
    auto webview2_color() const -> COREWEBVIEW2_COLOR;
    auto is_dark() const -> bool;

protected:
    winrt::UISettings uiSettings { glow::system::ui_settings() };
};
}; // namespace glow::color

// namespace std {
// template <> struct formatter<glow::color::Color> : formatter<string_view> {
//     auto format(const glow::color::Color& color, format_context& context) const noexcept {
//         return formatter<string_view>::format(glow::text::to_string(color.hex()), context);
//     }
// };

// template <> struct formatter<glow::color::Color, wchar_t> : formatter<wstring_view, wchar_t> {
//     auto format(const glow::color::Color& color, wformat_context& context) const noexcept {
//         return formatter<wstring_view, wchar_t>::format(glow::text::to_wstring(color.hex()),
//                                                         context);
//     }
// };
// } // namespace std
