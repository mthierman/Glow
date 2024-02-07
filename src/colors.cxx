// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include "colors.hxx"

namespace glow
{
SystemColors::SystemColors() : settings{winrt::Windows::UI::ViewManagement::UISettings()}
{
    update();
}

auto SystemColors::update() -> void
{
    color.accent = settings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::Accent);
    color.accentDark1 =
        settings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark1);
    color.accentDark2 =
        settings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark2);
    color.accentDark3 =
        settings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark3);
    color.accentLight1 =
        settings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight1);
    color.accentLight2 =
        settings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight2);
    color.accentLight3 =
        settings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight3);
    color.background =
        settings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::Background);
    color.foreground =
        settings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::Foreground);

    string.accent = to_string(color.accent);
    string.accentDark1 = to_string(color.accentDark1);
    string.accentDark2 = to_string(color.accentDark2);
    string.accentDark3 = to_string(color.accentDark3);
    string.accentLight1 = to_string(color.accentLight1);
    string.accentLight2 = to_string(color.accentLight2);
    string.accentLight3 = to_string(color.accentLight3);
    string.background = to_string(color.background);
    string.foreground = to_string(color.foreground);

    colorref.accent = to_colorref(color.accent);
    colorref.accentDark1 = to_colorref(color.accentDark1);
    colorref.accentDark2 = to_colorref(color.accentDark2);
    colorref.accentDark3 = to_colorref(color.accentDark3);
    colorref.accentLight1 = to_colorref(color.accentLight1);
    colorref.accentLight2 = to_colorref(color.accentLight2);
    colorref.accentLight3 = to_colorref(color.accentLight3);
    colorref.background = to_colorref(color.background);
    colorref.foreground = to_colorref(color.foreground);

    systemColors.clear();
    systemColors.emplace("accent", string.accent);
    systemColors.emplace("accentDark1", string.accentDark1);
    systemColors.emplace("accentDark2", string.accentDark2);
    systemColors.emplace("accentDark3", string.accentDark3);
    systemColors.emplace("accentLight1", string.accentLight1);
    systemColors.emplace("accentLight2", string.accentLight2);
    systemColors.emplace("accentLight3", string.accentLight3);
    systemColors.emplace("background", string.background);
    systemColors.emplace("foreground", string.foreground);
}

auto SystemColors::to_string(winrt::Windows::UI::Color uiColor) -> std::string
{
    return std::format("#{:0>2x}{:0>2x}{:0>2x}{:0>2x}", uiColor.R, uiColor.G, uiColor.B, uiColor.A);
}

auto SystemColors::to_colorref(winrt::Windows::UI::Color uiColor) -> ::COLORREF
{
    return RGB(std::ranges::clamp(static_cast<int>(uiColor.R), 0, 255),
               std::ranges::clamp(static_cast<int>(uiColor.G), 0, 255),
               std::ranges::clamp(static_cast<int>(uiColor.B), 0, 255));
}
} // namespace glow
