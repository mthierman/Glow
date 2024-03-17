// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#include <random>
#include <type_traits>

namespace glow
{
template <typename T = size_t, typename R = std::mt19937_64> auto random()
{
    constexpr T max{std::numeric_limits<T>::max()};
    thread_local R generator{std::random_device{}()};

    if constexpr (std::is_integral_v<T>)
    {
        thread_local std::uniform_int_distribution<T> dist(0, max);

        return dist(generator);
    }

    else if constexpr (std::is_floating_point_v<T>)
    {
        thread_local std::uniform_real_distribution<T> dist(0, max);

        return dist(generator);
    }
}
} // namespace glow
