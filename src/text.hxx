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

#include <random>
#include <string>
#include <type_traits>

#include <wil/win32_helpers.h>

#include "system.hxx"

namespace glow
{
namespace text
{
auto safe_size(size_t size) -> int;
auto narrow(std::wstring utf16) -> std::string;
auto widen(std::string utf8) -> std::wstring;
auto randomize(std::string string) -> std::string;
auto guid() -> GUID;
auto guid_string(GUID guid) -> std::string;

template <typename T, typename R = std::mt19937_64> auto random()
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
