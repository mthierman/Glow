// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <Windows.h>

#include <map>
#include <string>

#include <winrt/Windows.Foundation.h>

#include <winrt/Windows.UI.ViewManagement.h>

#include <nlohmann/json.hpp>

namespace winrt {
using namespace winrt::Windows::UI;
using namespace winrt::Windows::UI::ViewManagement;
}; // namespace winrt

namespace glow::color {
auto get_ui_settings() -> winrt::UISettings;
auto get_color(winrt::UISettings& uiSettings,
               winrt::UIColorType colorType = winrt::UIColorType::Accent) -> winrt::Color;
}; // namespace glow::color
