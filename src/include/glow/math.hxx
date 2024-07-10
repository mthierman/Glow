// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <limits>
#include <random>
#include <stdexcept>
#include <type_traits>

namespace glow::math {
template <typename T = int64_t, typename R = std::mt19937_64> auto make_random() -> T {
    constexpr T max { std::numeric_limits<T>::max() };
    thread_local R generator { std::random_device {}() };

    if constexpr (std::is_integral_v<T>) {
        thread_local std::uniform_int_distribution<T> dist(0, max);
        return dist(generator);
    }

    else if constexpr (std::is_floating_point_v<T>) {
        thread_local std::uniform_real_distribution<T> dist(0, max);
        return dist(generator);
    }
}

template <typename R, typename T> auto check_safe_size(T value) -> R {
    constexpr R max { std::numeric_limits<R>::max() };

    if (value > static_cast<T>(max)) {
        throw std::overflow_error("Unsafe size");
    }

    return static_cast<R>(value);
}
}; // namespace glow::math
