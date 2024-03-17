// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#include <cctype>
#include <cwctype>
#include <ranges>
#include <stdexcept>
#include <string>

namespace glow
{
auto string(std::wstring utf16) -> std::string;
auto wstring(std::string utf8) -> std::wstring;
auto trim(std::string string) -> std::string;

template <typename T, typename U> auto safe_size(T value) -> int
{
    constexpr int max{std::numeric_limits<U>::max()};
    if (value > static_cast<T>(max)) throw std::overflow_error("Unsafe size");

    return static_cast<U>(value);
}

template <typename T> auto lowercase(const T& string)
{
    if constexpr (std::is_same_v<T, std::string>)
    {
        return string |
               std::views::transform([](unsigned char c) -> unsigned char
                                     { return static_cast<unsigned char>(std::tolower(c)); }) |
               std::ranges::to<std::string>();
    }

    else if constexpr (std::is_same_v<T, std::wstring>)
    {
        return string |
               std::views::transform([](wchar_t c) -> wchar_t
                                     { return static_cast<wchar_t>(std::towlower(c)); }) |
               std::ranges::to<std::wstring>();
    }
}

template <typename T> auto uppercase(const T& string)
{
    if constexpr (std::is_same_v<T, std::string>)
    {
        return string |
               std::views::transform([](unsigned char c) -> unsigned char
                                     { return static_cast<unsigned char>(std::toupper(c)); }) |
               std::ranges::to<std::string>();
    }

    else if constexpr (std::is_same_v<T, std::wstring>)
    {
        return string |
               std::views::transform([](wchar_t c) -> wchar_t
                                     { return static_cast<wchar_t>(std::towupper(c)); }) |
               std::ranges::to<std::wstring>();
    }
}
} // namespace glow
