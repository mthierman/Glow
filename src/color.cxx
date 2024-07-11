// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/color.hxx>

namespace glow::color {
auto get_ui_settings() -> winrt::UISettings {
    return winrt::UISettings();
}

auto get_color(winrt::UISettings& uiSettings, winrt::UIColorType colorType) -> winrt::Color {
    return uiSettings.GetColorValue(colorType);
}
}; // namespace glow::color
