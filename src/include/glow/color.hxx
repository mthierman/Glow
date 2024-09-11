// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <Windows.h>

#include <format>
#include <string>

#include <winrt/Windows.Foundation.h>

#include <winrt/Windows.UI.ViewManagement.h>

namespace winrt {
using namespace winrt::Windows::UI;
using namespace winrt::Windows::UI::ViewManagement;
}; // namespace winrt

namespace glow::color {
auto system(winrt::UIColorType colorType) -> winrt::Color;
auto to_string(const winrt::Color& color) -> std::string;
auto to_wstring(const winrt::Color& color) -> std::wstring;
auto to_hex(const winrt::Color& color) -> std::string;
auto to_colorref(const winrt::Color& color) -> ::COLORREF;
}; // namespace glow::color

namespace std {
template <> struct formatter<winrt::Color> : formatter<string_view> {
    auto format(const winrt::Color& color, format_context& context) const noexcept {
        return formatter<string_view>::format(glow::color::to_string(color), context);
    }
};

template <> struct formatter<winrt::Color, wchar_t> : formatter<wstring_view, wchar_t> {
    auto format(const winrt::Color& color, wformat_context& context) const noexcept {
        return formatter<wstring_view, wchar_t>::format(glow::color::to_wstring(color), context);
    }
};
} // namespace std
