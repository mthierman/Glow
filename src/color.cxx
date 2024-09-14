// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/color.hxx>

#include <glow/system.hxx>
#include <glow/text.hxx>

namespace glow::color {
// auto Color::brush() -> ::HBRUSH { return ::CreateSolidBrush(RGB(r, g, b)); }

// auto Color::string() -> std::string {
//     return std::format("0x{:0>2x}{:0>2x}{:0>2x}{:0>2x}", r, g, b, a);
// }

auto create_brush(uint8_t r, uint8_t g, uint8_t b) -> ::HBRUSH {
    return ::CreateSolidBrush(RGB(r, g, b));
}

auto create_brush(const winrt::Color& color) -> ::HBRUSH {
    return ::CreateSolidBrush(RGB(color.R, color.G, color.B));
}

auto system(winrt::UIColorType colorType) -> winrt::Color {
    auto uiSettings { glow::system::ui_settings() };
    return uiSettings.GetColorValue(colorType);
}

auto element(winrt::UIElementType elementType) -> winrt::Color {
    auto uiSettings { glow::system::ui_settings() };
    return uiSettings.UIElementColor(elementType);
}

auto to_string(const winrt::Color& color) -> std::string {
    return std::format("R: {} G: {} B: {} A: {}", color.R, color.G, color.B, color.A);
}

auto to_wstring(const winrt::Color& color) -> std::wstring {
    return glow::text::to_wstring(to_string(color));
}

auto to_hex(const winrt::Color& color) -> std::string {
    return std::format("#{:0>2x}{:0>2x}{:0>2x}{:0>2x}", color.R, color.G, color.B, color.A);
}

auto to_colorref(const winrt::Color& color) -> ::COLORREF { return RGB(color.R, color.G, color.B); }

auto is_dark_mode() -> bool {
    auto fg { system(winrt::UIColorType::Foreground) };

    return (((5 * fg.G) + (2 * fg.R) + fg.B) > (8 * 128)) ? true : false;
}
}; // namespace glow::color
