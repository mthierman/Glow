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
auto to_utf8(std::wstring utf16) -> std::string;
auto to_utf16(std::string utf8) -> std::wstring;
auto make_guid() -> std::pair<GUID, std::string>;

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
auto append_random(std::string string) -> std::string;
} // namespace text
} // namespace glow
