// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#include <Windows.h>
#include <objbase.h>

#include <cctype>
#include <cwctype>
#include <random>
#include <ranges>
#include <string>
#include <type_traits>

#include <wil/win32_helpers.h>

#include <winrt/Windows.Foundation.h>

#include "system.hxx"

namespace glow
{
namespace text
{
auto to_utf8(std::wstring utf16) -> std::string;
auto to_utf16(std::string utf8) -> std::wstring;
auto append_random(std::string string) -> std::string;

template <typename T, typename U> auto safe_size(T value) -> int
{
    constexpr int max{std::numeric_limits<U>::max()};
    if (value > static_cast<T>(max)) throw std::overflow_error("Unsafe size");

    return static_cast<U>(value);
}

template <typename T> auto to_lower(const T& string)
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

template <typename T> auto to_upper(const T& string)
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

template <typename T, typename R = std::mt19937_64> auto make_random()
{
    constexpr T max{std::numeric_limits<T>::max()};
    R rng{std::random_device{}()};

    if constexpr (std::is_integral_v<T>)
    {
        std::uniform_int_distribution<T> dist(0, max);

        return dist(rng);
    }

    else if constexpr (std::is_floating_point_v<T>)
    {
        std::uniform_real_distribution<T> dist(0, max);

        return dist(rng);
    }
}
} // namespace text
} // namespace glow
