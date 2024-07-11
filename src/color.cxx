// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/color.hxx>
#include <glow/math.hxx>

#include <format>

namespace glow::color {
auto get_color(winrt::UISettings& uiSettings, winrt::UIColorType colorType) -> winrt::Color {
    return uiSettings.GetColorValue(colorType);
}

auto color_to_string(winrt::Color color) -> std::string {
    return std::format("#{:0>2x}{:0>2x}{:0>2x}{:0>2x}", color.R, color.G, color.B, color.A);
}

auto color_to_colorref(winrt::Color color) -> ::COLORREF { return RGB(color.R, color.G, color.B); }
}; // namespace glow::color
