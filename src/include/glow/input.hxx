// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <Windows.h>

#include <functional>
#include <initializer_list>
#include <unordered_map>

namespace glow::input {
auto was_key_down(int key) -> bool;
}; // namespace glow::input
