// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#include <Windows.h>
#include <map>
#include <string>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.ViewManagement.h>
#include <nlohmann/json.hpp>

namespace glow
{
struct Colors
{
    struct Color
    {
        winrt::Windows::UI::Color accent{};
        winrt::Windows::UI::Color accentDark1{};
        winrt::Windows::UI::Color accentDark2{};
        winrt::Windows::UI::Color accentDark3{};
        winrt::Windows::UI::Color accentLight1{};
        winrt::Windows::UI::Color accentLight2{};
        winrt::Windows::UI::Color accentLight3{};
        winrt::Windows::UI::Color background{};
        winrt::Windows::UI::Color foreground{};
    };

    struct String
    {
        std::string accent{};
        std::string accentDark1{};
        std::string accentDark2{};
        std::string accentDark3{};
        std::string accentLight1{};
        std::string accentLight2{};
        std::string accentLight3{};
        std::string background{};
        std::string foreground{};
    };

    struct Ref
    {
        ::COLORREF accent{};
        ::COLORREF accentDark1{};
        ::COLORREF accentDark2{};
        ::COLORREF accentDark3{};
        ::COLORREF accentLight1{};
        ::COLORREF accentLight2{};
        ::COLORREF accentLight3{};
        ::COLORREF background{};
        ::COLORREF foreground{};
    };

    Colors();

    auto update() -> void;
    auto to_string(winrt::Windows::UI::Color uiColor) -> std::string;
    auto to_colorref(winrt::Windows::UI::Color uiColor) -> ::COLORREF;
    auto clamp(int value) -> int;
    auto rgb_to_colorref(int r, int g, int b) -> ::COLORREF;

    winrt::Windows::UI::ViewManagement::UISettings settings{};
    Color color{};
    String string{};
    Ref ref{};
    std::map<std::string, std::string> colors{};

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Colors, colors)
};
} // namespace glow
