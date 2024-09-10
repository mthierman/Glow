// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/color.hxx>

#include <format>

#include <glow/math.hxx>
#include <glow/text.hxx>

namespace glow::color {
auto get_color(winrt::UISettings& uiSettings, winrt::UIColorType colorType) -> winrt::Color {
    return uiSettings.GetColorValue(colorType);
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
}; // namespace glow::color
